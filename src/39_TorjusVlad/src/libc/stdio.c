#include "libc/stdarg.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "arch/i386/console.h"
#include "libc/stdio.h"

int puts(const char* str) {
    if (!str) return -1;
    console_write(str);
    console_write("\n");
    return 0;
}

static void print_number(int value, int base) {
    char buffer[32];
    const char* digits = "0123456789ABCDEF";
    int i = 0;
    bool is_negative = false;

    if (value == 0) {
        console_write_char('0');
        return;
    }

    if (value < 0 && base == 10) {
        is_negative = true;
        value = -value;
    }

    while (value != 0) {
        buffer[i++] = digits[value % base];
        value /= base;
    }

    if (is_negative) buffer[i++] = '-';

    while (--i >= 0) {
        console_write_char(buffer[i]);
    }
}

int printf(const char* __restrict__ format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'c':
                    console_write_char((char)va_arg(args, int));
                    break;
                case 's':
                    console_write(va_arg(args, const char*));
                    break;
                case 'd':
                    print_number(va_arg(args, int), 10);
                    break;
                case 'x':
                    print_number(va_arg(args, int), 16);
                    break;
                case '%':
                    console_write_char('%');
                    break;
                default:
                    console_write_char('%');
                    console_write_char(format[i]);
                    break;
            }
        } else {
            console_write_char(format[i]);
        }
    }

    va_end(args);
    return 0;
}