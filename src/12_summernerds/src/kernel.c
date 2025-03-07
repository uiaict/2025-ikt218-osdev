#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY (volatile uint16_t*)0xB8000

uint8_t summer_colours = {0x4, 0xE, 0x2, 0x9}; 
struct multiboot_info {uint32_t size; uint32_t reserved; struct multiboot_tag *first;};

void add_colours_when_write_to_terminal(const char* str) {
    volatile uint16_t* vga = VGA_MEMORY;
    for (int i = 0; str[i]; i++)
        vga[i] = (rainbow_colors[i % 4] << 8) | str[i];
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr)
{add_colours_when_write_to_terminal("Hello Summernerds!!!")return 0;}