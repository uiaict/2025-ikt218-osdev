
#include "libc/scrn.h"

// Funksjon for å skrive tekst til skjermen
void terminal_write(const char* str, uint8_t color) {
    uint16_t* vga_buffer = VGA_MEMORY;
    size_t i = 0;

    while (str[i] != '\0') {
        vga_buffer[i+993] = VGA_ENTRY(str[i], color);
        i++;
    }
}


void printf(const char* str) {
    terminal_write(str, VGA_COLOR(15, 0)); // Hvit tekst på svart bakgrunn
}
