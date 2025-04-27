#include "libc/stdint.h"
#include "libc/stddef.h"
#include <libc/stdbool.h>
#include "libc/stdio.h"

#include "gdt.h"        /* our new GDT setup */
#include "io.h"         /* VGA text helpers  */
#include "kernel_memory.h" /* kernel memory management */
#include "memory_layout.h" /* memory layout printing */
#include "pit.h"
#include "paging.h"
#include <multiboot2.h> /* keep your existing boot header */
#include "interrupt.h"
#include "keyboard.h"
#include "kernel_main.h"


extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Define ISR handlers
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

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
    (void)magic;
    (void)mb_info_addr;

    /* 1. Install the Global Descriptor Table and switch segments */
    gdt_init();

    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();
    /* 2. Use VGA text mode to say hello */
    set_color(0x0A);  /* light-green on black                */
    puts("The byte of 33: GDT loaded!\n");

    init_idt();
    init_irq();

    // Register ISR handlers for test interrupts
    register_interrupt_handler(0, isr0_handler);
    register_interrupt_handler(1, isr1_handler);
    register_interrupt_handler(2, isr2_handler);

    // Enable interrupts
    __asm__ volatile ("sti");

    // Test ISRs
    puts("Triggering ISR tests....\n");
    __asm__ volatile ("int $0"); // Trigger interrupt 0
    __asm__ volatile ("int $1"); // Trigger interrupt 1
    __asm__ volatile ("int $2"); // Trigger interrupt 2

    // Ensure IRQ1 is enabled after ISR tests
    outb(0x21, 0xFC); // 0xFC = 11111110, enable IRQ0 and IRQ1

    puts("Type on the keyboard to see characters....\n");

    /* Call kernel_main (idk why) */
    return kernel_main();
}