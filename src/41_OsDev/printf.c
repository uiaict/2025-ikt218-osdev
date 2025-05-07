#include "printf.h"
#include "terminal.h"
#include <stdarg.h>
#include <stdint.h>

static void print_number(int value, int base) {
    char buffer[32];
    const char* digits = "0123456789ABCDEF";
    int i = 0;
    if (value == 0) {
        terminal_putchar('0');
        return;
    }
    if (value < 0 && base == 10) {
        terminal_putchar('-');
        value = -value;
    }
    while (value && i < 32) {
        buffer[i++] = digits[value % base];
        value /= base;
    }
    while (--i >= 0) {
        terminal_putchar(buffer[i]);
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    print_number(va_arg(args, int), 10);
                    break;
                case 'x':
                    print_number(va_arg(args, int), 16);
                    break;
                case 's':
                    terminal_write(va_arg(args, const char*));
                    break;
                case '%':
                    terminal_putchar('%');
                    break;
                default:
                    terminal_putchar('?');
            }
        } else {
            terminal_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}
