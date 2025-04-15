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
#include "memory/memory.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // Henta fra linker.ld

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Calculate end of kernel as the end of multiboot info structure initially
    // Initialize memory system with this preliminary end pointer
    terminal_initialize();

    init_kernel_memory(&end);
    init_paging();
    
    gdt_init();
    printf("GDT initialized successfully\n");
    
    idt_init();
    printf("IDT initialized successfully\n");
    
    isrs_install();
    irq_install();
    asm volatile("sti");

    printf("Kronos OS initialized\n");

    // Register handler for breakpoint exception
    register_interrupt_handler(3, print_interrupts);
    
    printf("=== Basic Output Tests ===\n");
    printf("Hei %x\n", 42);
    printf("Hei %u\n", 0x0F);
    printf("Hei %s\n", "Amund");
    printf("Hei %c\n", 'A');
    printf("pi = %f\n", 3.14);
    printf("pi = %f.2\n", 3.14);
    printf("pi = %f.0\n", 3.14);

    keyboard_init();
    printf("Keyboard logger active - Start typing\n\n");
    
    void *mem1 = malloc(1000);
    print_memory_layout();

    // Infinite loop to keep the kernel running
    for(;;) {
        // Halt CPU until next interrupt
        asm volatile("hlt");
    }
    
    return 0;
}
