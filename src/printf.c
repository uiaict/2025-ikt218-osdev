#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "printf.h"

// VGA
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*)0xB8000)

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t color = 0x07; 

static void move_cursor() {
    
}

void putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const size_t index = cursor_y * VGA_WIDTH + cursor_x;
        VGA_MEMORY[index] = (color << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0; 
    }

    move_cursor();
}

void puts(const char* str) {
    while (*str) {
        putc(*str++);
    }
}

static void print_decimal(int value) {
    char buffer[16];
    int i = 0;

    if (value == 0) {
        putc('0');
        return;
    }

    if (value < 0) {
        putc('-');
        value = -value;
    }

    while (value && i < 16) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--) {
        putc(buffer[i]);
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (const char* p = fmt; *p; p++) {
        if (*p != '%') {
            putc(*p);
            continue;
        }

        p++;
        switch (*p) {
            case 's': {
                const char* str = va_arg(args, const char*);
                puts(str);
                break;
            }
            case 'd': {
                int val = va_arg(args, int);
                print_decimal(val);
                break;
            }
            case 'c': {
                char c = (char)va_arg(args, int); 
                putc(c);
                break;
            }
            case '%': {
                putc('%');
                break;
            }
            default:
                putc('?');
        }
    }

    va_end(args);
}