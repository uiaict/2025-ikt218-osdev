#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"
#include "terminal.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <string.h>

// Main function of the kernel, called after bootloader passes control
int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    (void)magic;         // Ignore unused parameters for now
    (void)mb_info_addr;

    terminal_initialize();  // Clear the screen and prepare VGA memory
    terminal_setcolor(0x0A); // Green text color

    terminal_write("HELLO WORLD from terminal_write!\n", strlen("HELLO WORLD from terminal_write!\n")); // Assignment 2 output
    terminal_printf("HELLO WORLD from terminal_printf!\n");  // Assignment 2 output
    terminal_printf("Group 9 is here!\n\n");     // Assignment 2 output

    gdt_install();          // Install the Global Descriptor Table (GDT)
    terminal_printf("[OK] GDT installed.\n");

    idt_install();          // Install the Interrupt Descriptor Table (IDT)
    terminal_printf("[OK] IDT installed.\n");

    keyboard_install();    // Install the keyboard IRQ handler (IRQ1)
    terminal_printf("[OK] Keyboard IRQ installed.\n");

    terminal_printf("[+] Kernel initialization complete!\n\n");

    // Manually trigger interrupt
    __asm__ __volatile__("int $0x0");  // Test interrupt 0
    __asm__ __volatile__("int $0x2");  // Test interrupt 3
    __asm__ __volatile__("int $0x3");  // Test interrupt 2


    // Halt the CPU indefinitely
    while (1) {
        __asm__ __volatile__("hlt");
    }
}