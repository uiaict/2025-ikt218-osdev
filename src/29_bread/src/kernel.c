#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <gdt/gdt.h>
#include <libc/idt.h>
#include <libc/isr.h>
#include <print.h>
#include <putchar.h>
#include <libc/irq.h>
#include <keyboard.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// A simple test handler for IRQ debugging
void test_irq_handler(registers_t regs) {
    printf("TEST IRQ HANDLER CALLED FOR IRQ %d\n", regs.int_no - 32);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();

    // Set up GDT before IDT
    printf("Initializing GDT...\n");
    gdt_install();

    // Initialize the IDT
    printf("Initializing IDT...\n");
    init_idt();
    
    printf("Initializing IRQ system...\n");
    init_irq();
    
    // Make sure interrupts are disabled while setting up handlers
    asm volatile("cli");
    
    
    init_keyboard();
    
    // Manual test for keyboard handler
    registers_t dummy_regs;
    dummy_regs.int_no = 33; // IRQ 1 = INT 33
    printf("Manually triggering keyboard IRQ handler...\n");
    handle_irq(dummy_regs);
    
    printf("----------------------------------\n");
    printf("DEBUG: About to enable interrupts\n");
    
    // Enable interrupts after all handlers are registered
    asm volatile("sti");
    printf("Interrupts enabled\n");
    
    printf("System is running. Type on the keyboard...\n");
    
    // Safe infinite loop to prevent CPU from executing random memory
    while(1) {
        asm volatile("hlt");
    }

    return 0;
}