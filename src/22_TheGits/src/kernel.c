#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdarg.h"
#include "libc/gdt.h" 
#include "libc/scrn.h"



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    
    printf("Hello, World!");
  
    return 0;

}