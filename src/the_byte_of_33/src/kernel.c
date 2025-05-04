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
#include "keyboard.h"
#include "kernel_main.h"
#include "matrix_mode.h"
#include "piano_mode.h"

#include "kernel/boot_art.h"

extern uint32_t end;


static void isr0_handler(registers_t* r) {
    (void)r; // Ignore the dummy registers
    puts("Interrupt 0 (Divide by Zero) handled\n");
}

static void isr1_handler(registers_t* r) {
    (void)r;
    puts("Interrupt 1 (Debug) handled\n");
}

static void isr2_handler(registers_t* r) {
    (void)r;
    puts("Interrupt 2 (NMI) handled\n");
}

void init_keyboard_controller(void) {
    // Disable devices (PS/2 ports)
    outb(0x64, 0xAD); // Disable first PS/2 port (keyboard)
    outb(0x64, 0xA7); // Disable second PS/2 port (mouse, if present)

    // Flush the output buffer
    while (inb(0x64) & 0x01) {
        inb(0x60); // Read and discard
    }

    // Enable keyboard interrupt (IRQ1) in the controller
    outb(0x64, 0x60); // Write to configuration byte
    outb(0x60, 0x41); // Enable first PS/2 port interrupt, disable translation

    // Enable the keyboard
    outb(0x64, 0xAE); // Enable first PS/2 port (keyboard)

    // Reset the keyboard
    outb(0x60, 0xFF); // Send reset command to keyboard
    // Wait for ACK (optional, for robustness)
    while (!(inb(0x64) & 0x01)); // Wait for output buffer full
    uint8_t ack = inb(0x60);
    if (ack != 0xFA) {
        printf("Keyboard reset failed: ACK=0x%x\n", ack);
    }
}

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



// Music stub, quits on Q
/*static void music_mode(void) {
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
}*/


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
    init_keyboard_controller(); // Add this
    
    register_interrupt_handler(0, isr0_handler);
    register_interrupt_handler(1, isr1_handler);
    register_interrupt_handler(2, isr2_handler);
    register_interrupt_handler(33, keyboard_handler);

    puts("Triggering ISR tests....\n");
    __asm__ volatile("int $0");
    __asm__ volatile("int $1");
    __asm__ volatile("int $2");
    __asm__ volatile("sti");
    uint32_t eflags;
    __asm__ volatile("pushf; pop %0" : "=r"(eflags));
    printf("Interrupts enabled: %s\n", (eflags & 0x200) ? "Yes" : "No");
    outb(0x21, 0xFC);  // unmask timer (IRQ0) and keyboard (IRQ1)

    return kernel_main();
}
