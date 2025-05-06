#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <terminal.h>
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "irq.h"
#include "keyboard.h"
#include "ports.h"


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    gdt_install();
    idt_install();
    isr_install();
    pic_remap();
    irq_install();

    keyboard_install();
    
    terminal_initialize();
    terminal_writestring("Hello, World!\n");

    __asm__ __volatile__("sti");

    while (1) {
        __asm__ __volatile__("hlt");
    }
    
    return 0;

}