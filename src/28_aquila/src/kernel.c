
#include "gdt.h" 
#include "idt.h" 
#include "printf.h" 
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

// extern uint32_t end; // Task 1 - define

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {

    init_gdt();
    init_idt();
    init_pit(100); 

    // Clear previous buffer
    while (inb(0x64) & 0x01) {
        inb(0x60); 
    }
    asm volatile("sti");

    printf("Hello, Aquila!\n");
    printf("aquila: ");

    while (1) {
        asm volatile("hlt"); 
    }
    return 0;
}