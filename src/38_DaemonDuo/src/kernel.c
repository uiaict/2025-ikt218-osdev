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
    gdt_install(); // Initialize the GDT
    terminal_initialize(); // Initialize the terminal

    //install irq and idt
    idt_install();
    enable_irq(1); // Enable only keyboard IRQ for testing
    __asm__ __volatile__("sti"); // Enable interrupts globally




    writeline("Hello World\n"); // Print to the terminal

    
    while(true) {
        __asm__ __volatile__("hlt"); // Halt the CPU until an interrupt occurs
    }
}