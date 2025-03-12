#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};



void kernel_main() {
    // Initialize the GDT
    init_gdt();

    // Print "Hello World" to the terminal
    terminal_write("Hello World\n");
}

void init_gdt() {
    load_gdt();
}
