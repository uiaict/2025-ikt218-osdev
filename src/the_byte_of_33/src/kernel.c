// src/kernel.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include <libc/stdbool.h>
#include "libc/stdio.h"

#include "gdt.h"
#include "io.h"
#include "kernel_memory.h"
#include "memory_layout.h"
#include "pit.h"
#include "paging.h"
#include <multiboot2.h>
#include "interrupt.h"
#include "keyboard.h"    // wait_scancode()
#include "kernel_main.h"

#include "kernel/boot_art.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};  // ← semicolon was missing

// Poll PS/2 status port until a scancode arrives, then return it
static uint8_t wait_scancode(void) {
    while (!(inb(0x64) & 1)) {
        __asm__ volatile("hlt");
    }
    return inb(0x60);
}

// Ignore break codes (>=0x80), return only make codes
static uint8_t wait_make(void) {
    uint8_t sc;
    do {
        sc = wait_scancode();
    } while (sc & 0x80);
    return sc;
}

// Matrix mode: disable IRQ handler, animate, quit on Q
static void matrix_mode(void) {
    __asm__ volatile("cli");  // disable all IRQs
    __clear_screen();
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0A;

    int drops[80];
    uint32_t lfsr = 0xBEEF;
    for (int c = 0; c < 80; c++) {
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
        drops[c] = lfsr % 25;
    }

    while (1) {
        for (int c = 0; c < 80; c++) {
            int r = drops[c] - 1;
            if (r >= 0 && r < 25)
                vga[r*80 + c] = (ATTR<<8) | ' ';
        }
        for (int c = 0; c < 80; c++) {
            lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
            char ch = (lfsr & 1u) ? '1' : '0';
            int r = drops[c];
            if (r >= 0 && r < 25)
                vga[r*80 + c] = (ATTR<<8) | (uint8_t)ch;
            drops[c] = (r + 1) % 25;
        }
        // non-blocking Q check
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);
            if ((sc & 0x7F) == 0x10) {  // 'Q'
                __clear_screen();
                __asm__ volatile("sti");  // re-enable IRQs
                return;
            }
        }
        for (volatile uint32_t d = 0; d < 5000000; d++)
            __asm__ volatile("nop");
    }
}

// Music stub, quits on Q
static void music_mode(void) {
    // 1) Mask only keyboard IRQ (IRQ1) in the PIC, leave timer IRQ0 enabled.
    uint8_t pic1_mask = inb(0x21);
    outb(0x21, pic1_mask | 0x02);  // mask bit1 = IRQ1

    __clear_screen();
    set_color(0x0E);
    puts("Music Player Menu (press Q to go back):\n");
    puts("  [P] Play/Pause\n");
    puts("  [N] Next Track\n");
    puts("  [Q] Back\n");

    uint8_t sc;
    do {
        sc = wait_make();
        // you can dispatch P/N here based on sc
    } while (sc != 0x10);  // 0x10 = 'Q'

    __clear_screen();

    // 2) Restore original PIC mask to re-enable keyboard IRQ
    outb(0x21, pic1_mask);
}

// Piano stub, quits on Q
static void piano_mode(void) {
    // Mask only keyboard IRQ, like above
    uint8_t pic1_mask = inb(0x21);
    outb(0x21, pic1_mask | 0x02);  // mask IRQ1

    __clear_screen();
    set_color(0x0E);
    puts("Piano Mode (press Q to go back):\n");
    puts("  [Keys] Play notes\n");
    puts("  [Q] Back\n");

    uint8_t sc;
    do {
        sc = wait_make();
        // map scancodes to notes here
    } while (sc != 0x10);  // 'Q'

    __clear_screen();
    outb(0x21, pic1_mask);  // unmask IRQ1
}

int main(uint32_t magic, struct multiboot_info *mb) {
    (void)magic; (void)mb;

    // 1) GDT
    gdt_init();

    // 2) Splash
    animate_boot_screen();

    // 3) ~5s busy-wait
    for (volatile uint32_t i = 0; i < 200000000; i++)
        __asm__ volatile("nop");

    // 4) Clear splash
    __clear_screen();

    // 5) Kernel init
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();

    // 6) Hello
    set_color(0x0A);
    puts("The byte of 33: GDT loaded!\n");

    // 7) Enable IRQs & unmask keyboard
    init_idt();
    init_irq();
    __asm__ volatile("sti");
    outb(0x21, 0xFC);  // unmask timer (IRQ0) and keyboard (IRQ1)

    // 8) Main menu loop
    while (1) {
        __clear_screen();
        set_color(0x0E);
        puts("\nSelect mode:\n");
        puts("  [i] Matrix mode\n");
        puts("  [m] Music player\n");
        puts("  [p] Piano mode\n");

        // non-blocking poll for i/m/p
        __asm__ volatile("cli");
        uint8_t sc;
        while (1) {
            if (inb(0x64) & 1) {
                sc = inb(0x60) & 0x7F;
                if (sc == 0x17 || sc == 0x32 || sc == 0x19)
                    break;
            }
        }
        __asm__ volatile("sti");

        // dispatch
        if (sc == 0x17)       matrix_mode();  // 'i'
        else if (sc == 0x32)  music_mode();   // 'm'
        else                  piano_mode();   // 'p'
    }

    return kernel_main();
}
