#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt.h"
#include "idt.h"
#include "terminal.h"
#include "memory.h"
#include "pit.h"

// Reference to the end of the kernel in memory
extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install(); // Initialize the GDT
    terminal_initialize(); // Initialize the terminal

    // Initialize memory management
    init_kernel_memory(&end);
    init_paging();
    
    // Print memory layout information
    print_memory_layout();
    
    // Initialize PIT
    init_pit();

    // Install IDT and enable keyboard interrupts
    idt_install();
    enable_irq(0); // Enable timer IRQ
    enable_irq(1); // Enable keyboard IRQ
    __asm__ __volatile__("sti"); // Enable interrupts globally

    writeline("Hello World\n"); // Print to the terminal

    // Test sleep function
    writeline("Sleeping for 2 seconds...\n");
    sleep_interrupt(2 * 59.7); // Sleep for 2 seconds
    writeline("Woke up!\n");
    
    while(true) {
        __asm__ __volatile__("hlt"); // Halt the CPU until an interrupt occurs
    }
}