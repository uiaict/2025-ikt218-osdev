#include "libc/scrn.h"
#include "libc/stdarg.h"

// Funksjon for å skrive tekst til skjermen
void terminal_write(const char* str, uint8_t color) {
    static size_t row = 0; // Nåværende rad
    static size_t col = 0; // Nåværende kolonne
    uint16_t* vga_buffer = VGA_MEMORY;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            // Flytt til neste linje
            row++;
            col = 0;
        } else {
            // Skriv tegnet til VGA-buffret
            size_t index = row * VGA_WIDTH + col;
            vga_buffer[index] = VGA_ENTRY(str[i], color);
            col++;

            // Hvis vi når slutten av linjen, flytt til neste linje
            if (col >= VGA_WIDTH) {
                row++;
                col = 0;
            }
        }

        // Hvis vi når slutten av skjermen, rull opp
        if (row >= VGA_HEIGHT) {
            // Rull skjermen opp
            for (size_t y = 1; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
                }
            }

            // Tøm siste linje
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = VGA_ENTRY(' ', color);
            }

            row = VGA_HEIGHT - 1;
        }
    }
}
void itoa(int num, char* str, int base) {
    int i = 0;
    int is_negative = 0;

    // Håndter negativt tall for base 10
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    // Konverter tall til streng
    do {
        char digit = "0123456789ABCDEF"[num % base];
        str[i++] = digit;
        num /= base;
    } while (num > 0);

    // Legg til negativt tegn hvis nødvendig
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverser strengen
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}


void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (const char* ptr = format; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && *(ptr + 1) == 'c') {
            char c = (char)va_arg(args, int);
            terminal_write(&c, VGA_COLOR(15, 0));
            ptr++; // Hopp over 'c'
        } else {
            // Skriv kun én karakter – unngå å skrive hele strengen igjen!
            char ch = *ptr;
            terminal_write(&ch, VGA_COLOR(15, 0));
        }
    }

    va_end(args);
}
