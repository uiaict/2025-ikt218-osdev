// kernel.c
#include "../libc/stdint.h"
#include "../libc/stddef.h"
#include "../libc/stdbool.h"
#include <multiboot2.h>
#include "../terminal.h"
#include "../gdt.h"

extern void idt_install();
extern void pic_remap(int offset1, int offset2); 

struct multiboot_info;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install();
    terminal_initialize();
    pic_remap(0x20, 0x28);   
    idt_install();

    terminal_write("Hello from the IDT-enabled kernel!\n");

    asm volatile ("sti");   

    asm volatile ("int $0"); 
    asm volatile ("int $1");
    asm volatile ("int $2");

    while (1)
        asm volatile ("hlt");

    return 0;
}
