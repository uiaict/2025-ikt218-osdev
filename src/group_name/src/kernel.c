#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

    const char* message = "Hello World";
    uint8_t color = 0x07;

    for (int i = 0; message[i] != '\0'; i++) {
        vga_buffer[i] = (uint16_t)message[i] | ((uint16_t)color << 8);
    }

    while (1) {
    }

    return 0;
}