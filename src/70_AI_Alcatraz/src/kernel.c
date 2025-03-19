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

    // Rydd skjermen
    clear_screen();

    // Skriv "Hello World" til skjermen
    printf("Hello World!\n");

    // Skriv litt debug-informasjon fra Multiboot (valgfritt)
    printf("Multiboot Magic: 0x%x\n", magic);
    printf("Multiboot Info Address: 0x%x\n", (uint32_t)mb_info_addr);

    

    return 0;

}