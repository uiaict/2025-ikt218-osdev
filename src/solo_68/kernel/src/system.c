#include <stdarg.h>         // For handling variable arguments
#include "terminal.h"       // Terminal output functions
#include "system.h"         // General system utilities

// Helper function to print an integer in decimal format
static void print_decimal(int value) {
    char buffer[32];
    int i = 30;
    buffer[31] = '\0';

    if (value == 0) {
        terminal_putchar('0');
        return;
    }

    int negative = 0;
    if (value < 0) {
        negative = 1;
        value = -value;
    }

    // Convert digits to characters in reverse order
    while (value && i) {
        buffer[i--] = '0' + (value % 10);
        value /= 10;
    }

    // Add minus sign for negative numbers
    if (negative) {
        buffer[i--] = '-';
    }

    terminal_write(&buffer[i + 1]);
}

// Helper function to print an unsigned integer in hexadecimal format
static void print_hex(unsigned int value) {
    char buffer[32];
    int i = 30;
    buffer[31] = '\0';

    if (value == 0) {
        terminal_write("0x0");
        return;
    }

    // Convert each nibble to a hex digit in reverse order
    while (value && i) {
        int digit = value & 0xF;
        if (digit < 10)
            buffer[i--] = '0' + digit;
        else
            buffer[i--] = 'A' + (digit - 10);
        value >>= 4;
    }

    // Add "0x" prefix
    buffer[i--] = 'x';
    buffer[i--] = '0';

    terminal_write(&buffer[i + 1]);
}

// Simple implementation of printf supporting %d, %x, and %s
void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            if (format[i] == 'd') {
                int num = va_arg(args, int);
                print_decimal(num);
            } else if (format[i] == 'x') {
                unsigned int num = va_arg(args, unsigned int);
                print_hex(num);
            } else if (format[i] == 's') {
                char* str = va_arg(args, char*);
                terminal_write(str);
            } else {
                // Handle unknown format specifier
                terminal_putchar('%');
                terminal_putchar(format[i]);
            }
        } else if (format[i] == '\n') {
            // Convert newline to carriage return + newline for terminal compatibility
            terminal_putchar('\r');
            terminal_putchar('\n');
        } else if (format[i] == '\b') {
            // Handle backspace: erase previous character
            terminal_putchar('\b');
            terminal_putchar(' ');
            terminal_putchar('\b');
        } else {
            // Regular character output
            terminal_putchar(format[i]);
        }
    }

    va_end(args);
}
