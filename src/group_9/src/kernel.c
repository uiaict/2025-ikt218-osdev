#include "gdt.h"
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

    terminal_initialize();  // Initialize the VGA text mode terminal

    // Print messages using both terminal_write and terminal_printf
    terminal_write("HELLO WORLD from terminal_write!\n", strlen("HELLO WORLD from terminal_write!\n"));
    terminal_printf("HELLO WORLD from terminal_printf!\n");
    terminal_printf("Number example: %d\n", 1234);
    terminal_printf("String example: %s\n", "Operating System");

    // Halt the CPU indefinitely
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

