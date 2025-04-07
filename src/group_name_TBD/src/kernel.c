#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include <multiboot2.h>
#include "descriptor_tables.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t my_struct, uint32_t magic, struct multiboot_info *mb_info_addr) {
    
    init_gdt();
    init_idt();

    typedef struct{
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e[6];
    } MyStruct;

    MyStruct *abc = (MyStruct*)my_struct;
    
    const char *string = "xxx\rabc\ndef\r\nxyz\r\n";
    set_vga_color(VGA_RED, VGA_BLUE);
    printf(string);
    printf("dddd");
    char t = 'T';
    set_vga_color(VGA_WHITE, VGA_BLACK);
    putchar_at(&t, 10, 10);
    
    // asm volatile ("int $0x1E");
    // asm volatile ("int $0x1F"); 
    // asm volatile ("int $0x20");
    // asm volatile ("int $0x21"); 

    return 0;

}