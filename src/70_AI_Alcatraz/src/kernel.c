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


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Initialiser GDT
    gdt_init();

    clear_screen();
    printf("Hello, Kernel!\n");
    printf("Number: %d\n", 42);
    printf("Character: %c\n", 'A');
    printf("String: %s\n", "Operating System");

    

    return 0;

}