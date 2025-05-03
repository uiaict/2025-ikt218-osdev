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
    __asm__ volatile("sti");
    outb(0x21, 0xFC);  // unmask timer (IRQ0) and keyboard (IRQ1)

    return kernel_main();
}
