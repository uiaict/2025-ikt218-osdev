#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <gdt/gdt.h>
#include <libc/idt.h>
#include <libc/isr.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    init_idt();

    gdt_install();

    asm volatile("cli");
    asm volatile("int $0x0");

    while(1);
    return 0;

}