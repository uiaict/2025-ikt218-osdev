#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt/gdt.h"
#include <libc/stdarg.h>
#include "vga/vga.h"
#include "util/util.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    printf("hello, %c, %s, %i, %f", 'G', "yay bro",15,15.555555555555);
    return 0;
}