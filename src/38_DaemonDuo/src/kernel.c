#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt.h"
#include "idt.h"
#include "terminal.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install();
    terminal_initialize();
    writeline("Hello World\n");

    idt_install();
    
    // Enable only keyboard IRQ for testing
    enable_irq(1);
    
    // Enable interrupts globally
    __asm__ __volatile__("sti");

    // Test only keyboard interrupt
    writeline("Testing keyboard interrupt (33)...\n");
    __asm__ __volatile__("int $33");

    __asm__ __volatile__("int $1");

    writeline("Test complete. Press any key for hardware interrupt.\n");
    
    while(true) {
        __asm__ __volatile__("hlt");
    }
}