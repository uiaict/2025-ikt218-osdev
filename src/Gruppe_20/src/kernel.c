#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#include <multiboot2.h>
#include "Descriptor_tables/gdt_idt_table.h"
#include "Descriptor_tables/print.h"
#include "Descriptor_tables/gdt.h"
#include "Descriptor_tables/idt.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    init_idt();
    printf("Hello %s", "World");


    return 0;

}