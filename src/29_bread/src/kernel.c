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
#include "memory.h"  // Changed from <memory.h>

extern uint32_t end; // Linker-provided symbol for end of kernel

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

    printf("initializing kernel memory...\n");
    init_kernel_memory((uint32_t*)&end); // You'll need to define 'end' as extern
    printf("Kernel memory initialized\n");
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
    
   init_kernel_memory(&end);
   init_paging();
   print_memory_layout();


    printf("System is running. Type on the keyboard...\n");

    // Safe infinite loop to prevent CPU from executing random memory
    while(1) {
        asm volatile("hlt");
    }

    return 0;
}
