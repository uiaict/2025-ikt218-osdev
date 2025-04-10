#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <gdt/gdt.h>
#include <libc/idt.h>
#include <libc/isr.h>
#include <print.h>
#include <putchar.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();

    // Set up GDT before IDT
    printf("Initializing GDT...\n");
    gdt_install();

    // Initialize the IDT
    printf("Initializing IDT...\n");
    init_idt();
    
    printf("System initialized successfully!\n");

    asm volatile("int $0x3");

    // Safe infinite loop to prevent CPU from executing random memory
    printf("System is running. Press Ctrl+Alt+Del to restart.\n");
    while(1) {
        asm volatile("hlt");
    }

    return 0;
}