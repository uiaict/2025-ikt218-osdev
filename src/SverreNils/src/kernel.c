#include "arch/gdt.h"
#include "printf.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void putc_raw(char c) {
    volatile char* video = (volatile char*)0xB8000;
    video[0] = c;
    video[1] = 0x07;
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    putc_raw('Z');
    gdt_init();
    printf("Hello, Nils!\n");
    return 0;

}