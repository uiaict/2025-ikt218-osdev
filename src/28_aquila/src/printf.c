#include "libc/stdarg.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "printf.h"



volatile char *vga = (volatile char *)VGA_ADDRESS;
int cursor = 0;

extern void outb(uint16_t port, uint8_t value);

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x07;
    }
    cursor = 0;
    update_cursor(0, 0);
}


void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
  }

void scroll() {
    // copy each line one row up
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int from = (y * VGA_WIDTH + x) * 2;
            int to = ((y - 1) * VGA_WIDTH + x) * 2;
            vga[to] = vga[from];
            vga[to + 1] = vga[from + 1];
        }
    }

    // clear the last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        int i = ((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2;
        vga[i] = ' ';
        vga[i + 1] = 0x07;
    }

    cursor = (VGA_HEIGHT - 1) * VGA_WIDTH;
}


void putchar(char c) {
    if (c == '\n') {
        cursor += VGA_WIDTH - (cursor % VGA_WIDTH); // move to start of next line
    } else if (c == '\b') {
        if (cursor > 0) {
            cursor--;
            vga[cursor * 2] = ' ';
            vga[cursor * 2 + 1] = 0x07; 
        }

    } else {
        vga[cursor * 2] = c;
        vga[cursor * 2 + 1] = 0x07;
        cursor++;
    }

    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
        scroll();
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

    int x = cursor % VGA_WIDTH;
    int y = cursor / VGA_WIDTH;
    update_cursor(x, y);
}
