#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* const video_memory = (uint16_t*) VGA_ADDRESS;
static size_t row = 0;
static size_t col = 0;
static uint8_t color = 0x07; 
static void scroll(); 


int putchar(int ic) {
    char c = (char)ic;

    if (c == '\n') {
        row++;
        col = 0;
    } else {
        video_memory[row * VGA_WIDTH + col] = ((uint16_t)color << 8) | c;
        col++;

        if (col >= VGA_WIDTH) {
            col = 0;
            row++;
        }
    }

    if (row >= VGA_HEIGHT) {
        scroll();
    }

    return ic;
}


static void scroll() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    // Blank ut siste linje
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((uint16_t)color << 8) | ' ';
    }

    row = VGA_HEIGHT - 1;
    col = 0;
}


bool print(const char* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        putchar(data[i]);
    }
    return true;
}

static void print_dec(int value) {
    char buffer[16];
    int i = 0;
    bool negative = value < 0;
    if (negative) value = -value;

    do {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    if (negative) buffer[i++] = '-';

    while (i--) putchar(buffer[i]);
}

static void print_hex(uint32_t value) {
    char buffer[9];
    buffer[8] = '\0';
    const char* hex = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex[value & 0xF];
        value >>= 4;
    }
    print(buffer, 8);
}

int printf(const char* __restrict__ format, ...) {
    va_list args;
    va_start(args, format);
    int written = 0;

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    while (*str) putchar(*str++), written++;
                    break;
                }
                case 'd':
                    print_dec(va_arg(args, int));
                    // Optional: count digits
                    break;
                case 'x':
                    print_hex(va_arg(args, uint32_t));
                    break;
                case 'c':
                    putchar(va_arg(args, int));
                    written++;
                    break;
                case '%':
                    putchar('%');
                    written++;
                    break;
                default:
                    putchar('%');
                    putchar(format[i]);
                    written += 2;
            }
        } else {
            putchar(format[i]);
            written++;
        }
    }

    va_end(args);
    return written;
}
