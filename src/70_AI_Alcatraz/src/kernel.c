#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "GDT.h"
#include "printf.h"
#include <multiboot2.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// No need to redefine outb here as it's already defined in printf.h

void some_function() {
    outb(0x20, 0x20); // Example usage of outb
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Initialiser GDT
    gdt_init();

    clear_screen();
    printf("Hello, Kernel!\n");
    printf("Hello World\n");

    return 0;

}