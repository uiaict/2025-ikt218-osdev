#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY (volatile uint16_t*)0xB8000

uint8_t rainbow_colours[4] = {0x4, 0xE, 0x2, 0x9}; // Rød, gul, grønn, blå

struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};

// 🎨 Skriver til terminalen linje for linje
void write_line_to_terminal(const char* str, int line) {
    if (line >= VGA_HEIGHT) return; // Unngår å skrive utenfor skjermen

    volatile uint16_t* vga = VGA_MEMORY + (VGA_WIDTH * line); // Flytter til riktig linje

    for (int i = 0; str[i] && i < VGA_WIDTH; i++) {
        vga[i] = (rainbow_colours[i % 4] << 8) | str[i]; // Skriver tegn med farge
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    write_line_to_terminal("Hello", 1);  // Første linje
    write_line_to_terminal("Summernerds!!!", 2);  // Andre linje

    return 0;
}
