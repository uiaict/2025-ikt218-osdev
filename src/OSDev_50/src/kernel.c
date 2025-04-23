#include <multiboot2.h>

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/system.h"


// #include "terminal.h"

#include "common.h"
#include "interrupts.h"
#include "descriptor_tables.h"
#include "input.h"
#include "monitor.h"
#include "gdt.h"

// Structure to hold multiboot information.
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



void kernel_main(void) {
    // Initialize terminal first
    // terminal_initialize();

    // Initialize monitor
    monitor_initialize();
    
    // Initialize GDT
    init_gdt();

    // Initialize IDT
    init_idt();

    // Initialize IRQ
    init_irq();

    // Print Hello World
    monitor_writestring("Hello World!\n");

    // Test the outb function
    test_outb();

    asm("int $0x0");

    
    
}
