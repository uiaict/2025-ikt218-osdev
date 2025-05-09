#include "libc/stdio.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdarg.h"

#define VIDEO_MEMORY ((uint8_t*)0xb8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA terminal cursor state
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x07;

static void scroll_terminal() {
    // Move each row up by one
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t from = (y * VGA_WIDTH + x) * 2;
            size_t to = ((y - 1) * VGA_WIDTH + x) * 2;
            VIDEO_MEMORY[to] = VIDEO_MEMORY[from];
            VIDEO_MEMORY[to + 1] = VIDEO_MEMORY[from + 1];
        }
    }

    // Clear the last row
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        size_t index = ((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2;
        VIDEO_MEMORY[index] = ' ';
        VIDEO_MEMORY[index + 1] = terminal_color;
    }

    terminal_row = VGA_HEIGHT - 1;
}

// --- put_entry_at: Write a character with color at screen coordinates (x, y) ---
void put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    size_t index = (y * VGA_WIDTH + x) * 2;
    VIDEO_MEMORY[index] = c;
    VIDEO_MEMORY[index + 1] = color;
}

// --- putchar: Print a character to the screen and handle newlines ---
int putchar(int c) {
    char ch = (char)c;

    if (ch == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) scroll_terminal();
        return c;
    }

    put_entry_at(ch, terminal_color, terminal_column, terminal_row);
    terminal_column++;

    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
       if (terminal_row >= VGA_HEIGHT) scroll_terminal();
    }

    return c;
}

// --- print_string: Print a null-terminated string ---
static int print_string(const char* str) {
    int count = 0;
    while (*str) {
        putchar(*str++);
        count++;
    }
    return count;
}

// --- print_unsigned: Print an unsigned integer in a given base (10 or 16) ---
static int print_unsigned(uint32_t value, int base) {
    if (value == 0) {
        putchar('0');
        return 1;
    }

    char buffer[32];
    const char* digits = "0123456789abcdef";
    int i = 0;

    while (value > 0) {
        buffer[i++] = digits[value % base];
        value /= base;
    }

    int count = 0;
    while (i > 0) {
        putchar(buffer[--i]);
        count++;
    }

    return count;
}

// --- print_signed: Print a signed integer using print_unsigned internally ---
static int print_signed(int32_t value) {
    if (value < 0) {
        putchar('-');
        return 1 + print_unsigned((uint32_t)(-value), 10);
    }
    return print_unsigned((uint32_t)value, 10);
}

// --- printf: Minimal formatted output supporting %s %c %d %u %x and %% ---
int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int chars_written = 0;

    for (size_t i = 0; format[i]; i++) {
        if (format[i] == '%' && format[i + 1]) {
            i++;  // Move to format specifier
            switch (format[i]) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (!str) str = "(null)";
                    chars_written += print_string(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);  // promoted to int in varargs
                    putchar(c);
                    chars_written++;
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    chars_written += print_signed(val);
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    chars_written += print_unsigned(val, 10);
                    break;
                }
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    chars_written += print_unsigned(val, 16);
                    break;
                }
                case '%': {
                    putchar('%');
                    chars_written++;
                    break;
                }
                default: {
                    // Handle unknown format: print it literally
                    putchar('%');
                    putchar(format[i]);
                    chars_written += 2;
                    break;
                }
            }
        } else {
            putchar(format[i]);
            chars_written++;
        }
    }

    va_end(args);
    return chars_written;
}
