
#include "gdt.h" 
#include "idt.h" 
#include "printf.h" 
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {

    init_gdt();
    init_idt();
    init_pit(100); 
    asm volatile("sti");

    while (1) {
        asm volatile("hlt"); 
    }
    return 0;
}