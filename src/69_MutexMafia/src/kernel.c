#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt/gdt.h"
#include "io/printf.h"
#include "idt/idt.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    initIdt();
    
    mafiaPrint("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    mafiaPrint("Yeeeee drugga\n");
    asm("int $0x0");
    //while (1){}
    return 0;

}