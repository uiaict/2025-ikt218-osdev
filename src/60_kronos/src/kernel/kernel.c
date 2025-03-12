#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include <multiboot2.h>

#include "kernel/gdt.h"
#include "drivers/terminal.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    gdt_init();
    printf("Hei %x\n", 42);
    printf("Hei %u\n", 0x0F);
    printf("Hei %s\n", "Amund");
    printf("Hei %c\n", 'A');
    printf("pi = %f\n", 3.14);

    printf("pi = %f.2\n", 3.14);
    printf("pi = %f.0\n", 3.14);
    
    return 0;

}
