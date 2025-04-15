#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt/gdt.h"
#include "io/printf.h"
#include "idt/idt.h"
#include "io/keyboard.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    initIdt();
    mafiaPrint("Yeeeee drugga\n");
    initKeyboard();
    
    //asm("int $0x0"); //testing av division by zero exception
    while (1){}
    return 0;

}