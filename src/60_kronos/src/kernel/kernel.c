#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/isr.h"
#include "drivers/terminal.h"
#include "drivers/keyboard.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    terminal_initialize();
    
    gdt_init();
    printf("GDT initialized successfully\n");
    
    idt_init();
    printf("IDT initialized successfully\n");
    
    isrs_install();
    irq_install();
    
    // Register handler for breakpoint exception
    register_interrupt_handler(3, custom_interrupt_handler);
    
    keyboard_init();
    
    printf("Kronos OS initialized\n");
    printf("Interrupt system ready\n");
    printf("Keyboard logger active - Start typing\n\n");
    
    // Enable interrupts
    asm volatile("sti");
    
    printf("=== Basic Output Tests ===\n");
    printf("Hei %x\n", 42);
    printf("Hei %u\n", 0x0F);
    printf("Hei %s\n", "Amund");
    printf("Hei %c\n", 'A');
    printf("pi = %f\n", 3.14);
    printf("pi = %f.2\n", 3.14);
    printf("pi = %f.0\n", 3.14);
    
    // Infinite loop to keep the kernel running
    for(;;) {
        // Halt CPU until next interrupt
        asm volatile("hlt");
    }
    
    return 0;
}
