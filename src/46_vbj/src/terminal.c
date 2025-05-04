#include <libc/stdint.h>
#include <libc/stdarg.h>
#include "global.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

uint16_t *vga_buffer = (uint16_t*)0xB8000;
uint16_t cursor_pos = 0;

void clear_screen() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_buffer[i] = ' ' | 0x0700;
    }
    cursor_pos = 0;
}

void print_char(char c) {
    if (c == '\n') {
        cursor_pos += SCREEN_WIDTH - (cursor_pos % SCREEN_WIDTH);
    } else {
        vga_buffer[cursor_pos++] = (uint16_t)c | 0x0700;
    }
    
    if (cursor_pos >= SCREEN_WIDTH * SCREEN_HEIGHT)
    {
        scroll_down();
        cursor_pos = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH;
    }

    move_cursor(cursor_pos);
}


void print_string(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_int(int num) {
    char buffer[20];  
    int i = 0;
    
    if (num == 0) {
        print_char('0');
        return;
    }

    if (num < 0) {
        print_char('-');
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }


    while (i > 0) {
        print_char(buffer[--i]);
    }
}

void print_hex(uint32_t num) {
    char buffer[9]; 
    int i = 7;
    buffer[8] = '\0';

    if (num == 0) {
        print_string("0x0");
        return;
    }

    while (num > 0) {
        uint8_t digit = num & 0xF;
        buffer[i--] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        num >>= 4;
    }

    print_string("0x");
    print_string(&buffer[i + 1]);
}

void move_cursor(uint16_t position) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}


void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    print_string(str);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    print_int(num);
                    break;
                }
                case 'x': {   // Ny støtte for heksadesimale tall
                    uint32_t num = va_arg(args, uint32_t);
                    print_hex(num);
                    break;
                }
                case 'c': {  
                    char c = (char)va_arg(args, int);
                    print_char(c);
                    break;
                }
                case '%': {
                    print_char('%');
                    break;
                }
                default: {
                    print_char('%');
                    print_char(*format);
                    break;
                }
            }
        } else {
            print_char(*format);
        }
        format++;
    }

    va_end(args);
}

void scroll_down() {
    for (int y = 1; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            vga_buffer[(y - 1) * SCREEN_WIDTH + x] = vga_buffer[y * SCREEN_WIDTH + x];
        }
    }
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        vga_buffer[(SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x] = ' ' | 0x0700;
    }
}
