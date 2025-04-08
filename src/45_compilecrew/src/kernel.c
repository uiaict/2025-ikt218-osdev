#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/terminal.h"
#include "libc/printf.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    int noop = 0;
    init_gdt();
    
    printf("Hello World\n");
    printf("Number: %d\n", 42);
    printf("Hex: %x\n", 255);
    printf("Character: %c\n", 'A');
    printf("String: %s\n", "Kernel Booted");
    
    return 0;

}