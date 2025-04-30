#include "libc/stdint.h"
#include "libc/stddef.h"
#include <libc/stdbool.h>
#include "libc/stdio.h"

#include "gdt.h"               /* our new GDT setup */
#include "io.h"                /* VGA text helpers  */
#include "kernel_memory.h"     /* kernel memory management */
#include "memory_layout.h"     /* memory layout printing */
#include "pit.h"               /* (unused here) */
#include "paging.h"
#include <multiboot2.h>        /* your existing boot header */
#include "interrupt.h"
#include "keyboard.h"
#include "kernel_main.h"

#include "kernel/boot_art.h"   /* animate_boot_screen(), __clear_screen() */

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Simple ISR handlers for testing
static void isr0_handler(registers_t* r) {
    (void)r;
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

// Stub: replace with your actual music‐menu implementation
static void draw_music_menu(void) {
    set_color(0x0E);
    puts("\nMusic Player Menu:\n");
    puts("  [P] Play/Pause\n");
    puts("  [N] Next Track\n");
    puts("  [Q] Quit\n");
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
    (void)magic;
    (void)mb_info_addr;

    /* 1. Install GDT */
    gdt_init();

    /* 2. Show animated ASCII splash */
    animate_boot_screen();

    /* 3. Busy‐wait approximately 5 seconds */
    for (volatile uint32_t i = 0; i < 200000000; i++) {
        __asm__ volatile("nop");
    }

    /* 4. Clear the splash */
    __clear_screen();

    /* 5. Continue original initialization */
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();  /* re-init PIT for later timing if needed */

    /* 6. VGA hello message */
    set_color(0x0A);  /* light-green on black */
    puts("The byte of 33: GDT loaded!\n");

    /* 7. Initialize IDT and IRQs */
    init_idt();
    init_irq();

    /* 8. Register ISR handlers for testing */
    register_interrupt_handler(0, isr0_handler);
    register_interrupt_handler(1, isr1_handler);
    register_interrupt_handler(2, isr2_handler);

    /* 9. Enable interrupts and fire test ISRs */
    __asm__ volatile("sti");
    puts("Triggering ISR tests....\n");
    __asm__ volatile("int $0");
    __asm__ volatile("int $1");
    __asm__ volatile("int $2");

    /* 10. Unmask keyboard IRQ */
    outb(0x21, 0xFC);  /* enable IRQ0 & IRQ1 */
    puts("Type on the keyboard to see characters....\n");

    /* 11. Draw music menu */
    draw_music_menu();

    /* 12. Hand off to C++ kernel_main (never returns) */
    return kernel_main();
}
