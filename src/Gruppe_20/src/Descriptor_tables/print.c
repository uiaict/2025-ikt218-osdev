#include "libc/print.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
// Screen dimensions
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Function to print a string to the screen
void print_string(const char* str) {
    static int cursor_x = 0;
    static int cursor_y = 0;
    volatile char* video = (volatile char*)0xB8000;

    while (*str) {
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            int offset = (cursor_y * SCREEN_WIDTH + cursor_x) * 2;
            video[offset] = *str;
            video[offset + 1] = 0x0F; // Light grey on black
            cursor_x++;
            
            // Move to the next line if cursor exceeds screen width
            if (cursor_x >= SCREEN_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }
        
        str++;
        
        // Handle screen overflow
        if (cursor_y >= SCREEN_HEIGHT) {
            cursor_y = 0; // Wrap around to the top of the screen
        }
    }
}

void print_char(char c) {
    char str[2] = {c, '\0'};
    print_string(str);
}

void print_int(int value) {
    char buffer[12]; // Enough to hold -2147483648
    char* p = buffer + 11;
    bool is_negative = value < 0;

    *p = '\0';

    if (is_negative) {
        value = -value;
    }

    do {
        *--p = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    if (is_negative) {
        *--p = '-';
    }

    print_string(p);
}

void vprintf(const char* format, va_list args) {
    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char* str = va_arg(args, char*);
                    print_string(str);
                    break;
                }
                case 'd': {
                    int value = va_arg(args, int);
                    print_int(value);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int); // char is promoted to int in varargs
                    print_char(c);
                    break;
                }
                case '%': {
                    print_char('%');
                    break;
                }
                default:
                    print_char('%');
                    print_char(*format);
                    break;
            }
        } else {
            print_char(*format);
        }
        format++;
    }
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
