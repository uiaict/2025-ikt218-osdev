#ifndef PRINTF_H
#define PRINTF_H

#include "libc/stdint.h"

// Definerer VGA-tekstmodus bufferadresse og skjermstørrelse
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

// Declare outb function first
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %%al, %%dx" : : "a"(val), "d"(port) );
}

// Globale variabler for terminalbuffer og markørposisjon
static unsigned short* terminal_buffer = (unsigned short*) VGA_ADDRESS;
static int cursor_x = 0, cursor_y = 0;

// Flytter markøren i VGA-tekstbufferet
static void move_cursor() {
    unsigned short pos = cursor_y * VGA_WIDTH + cursor_x;

    // Porter for VGA markør
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}

// Add a non-static prototype for move_cursor so it can be called from other files
void move_cursor();

// Tømmer skjermen
static void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = (WHITE_ON_BLACK << 8) | ' ';
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Skriver et tegn til skjermen
static void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        terminal_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;
        cursor_x++;
    }

    // Gå til neste linje om nødvendig
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    // Rens skjermen hvis vi når slutten
    if (cursor_y >= VGA_HEIGHT) {
        clear_screen();
    }

    move_cursor();
}

// Skriver en streng til skjermen
static void print(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

// Skriver et heltall til skjermen
static void print_int(int num) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }
    char buffer[10];
    int i = 0;
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num);

    while (i--) {
        putchar(buffer[i]);
    }
}

// En enkel printf-funksjon med %s, %d, %c, %%
static void printf(const char* format, ...) {
    char** arg_ptr = (char**) &format;
    arg_ptr++;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c':
                    putchar(*(char*)arg_ptr);
                    arg_ptr++;
                    break;
                case 's':
                    print(*(char**)arg_ptr);
                    arg_ptr++;
                    break;
                case 'd':
                    print_int(*(int*)arg_ptr);
                    arg_ptr++;
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    putchar(*format);
            }
        } else {
            putchar(*format);
        }
        format++;
    }
}

#endif // PRINTF_H
