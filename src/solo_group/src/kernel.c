#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string;
        *video++ = colour;
    }
}


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    char string[] = "T";
    write_string(3, string);
    
    return kernel_main();
}