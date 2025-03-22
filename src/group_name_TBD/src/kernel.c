#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t my_struct, uint32_t magic, struct multiboot_info *mb_info_addr) {

    typedef struct{
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e[6];
    } MyStruct;

    MyStruct *abc = (MyStruct*)my_struct;
    
    const char *string = "xxx\rabc\ndef\r\nxyz\r\n";
    set_vga_color(RED, BLUE);
    printf(string);
    printf("dddd");
    char t = 'T';
    set_vga_color(WHITE, BLACK);
    putchar_at(&t, 10, 10);
    

    return 0;

}