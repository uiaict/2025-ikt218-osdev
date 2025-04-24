#include "libc/stdio.h"
#include "libc/stdint.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void kmain(uint32_t magic, struct multiboot_info* mb_info_addr) {
    printf("Hello World\n");
    while (1);
}
