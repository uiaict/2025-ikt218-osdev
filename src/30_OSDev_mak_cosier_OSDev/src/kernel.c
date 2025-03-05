#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "../include/teminal.h"
#include "../include/libc/gdt.h"
#include "../include/libc/idt.h"



struct multiboot_info 
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{

    printf("Hello, World!\n");

    init_gdt();
    init_idt();

    int i = 1/0;
    asm volatile ("int $0x03");

    print_number(12345);
    printf("\n");
    print_number(-9876); 
    printf("\n"); 
    print_number(0);      

    while(1);
    return 0;
}

