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
    reset();
    print("hello world!\nfuuck\n%c\n%s\n%d\n%f",'F',"check this string out babyyyy",15,15.555555555);
    return 0;
}