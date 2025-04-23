#include "libc/stdarg.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *vga = (volatile char *)VGA_ADDRESS;
int cursor = 0;

void putchar(char c) {
    if (c == '\n') {
        cursor += VGA_WIDTH - (cursor % VGA_WIDTH); // Move to start of next line
    } else {
        vga[cursor * 2] = c;
        vga[cursor * 2 + 1] = 0x07;
        cursor++;
    }

    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
        cursor = 0; // Wrap (or implement scrolling here)
    }
}

void print_decimal(int n) {
    char buf[12];
    int i = 0;
    if (n == 0) {
        putchar('0');
        return;
    }
    if (n < 0) {
        putchar('-');
        n = -n;
    }
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    while (--i >= 0) putchar(buf[i]);
}

void print_hex(uint32_t n) {
    char hex[] = "0123456789ABCDEF";
    putchar('0'); putchar('x');
    for (int i = 28; i >= 0; i -= 4) {
        putchar(hex[(n >> i) & 0xF]);
    }
}

void print_string(const char* str) {
    while (*str) putchar(*str++);
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1]) {
            i++;
            switch (fmt[i]) {
                case 'd': print_decimal(va_arg(args, int)); break;
                case 'x': print_hex(va_arg(args, uint32_t)); break;
                case 's': print_string(va_arg(args, char*)); break;
                case 'c': putchar((char)va_arg(args, int)); break;
                default: putchar('%'); putchar(fmt[i]); break;
            }
        } else {
            putchar(fmt[i]);
        }
    }

    va_end(args);
}
