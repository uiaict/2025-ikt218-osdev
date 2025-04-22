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

static void print_number(unsigned int value, int base, bool is_signed) {
    char buffer[32];
    const char* digits = "0123456789ABCDEF";
    int i = 0;

    // Handle signed negative numbers in base 10
    if (is_signed && (int)value < 0 && base == 10) {
        value = (unsigned int)(-(int)value);
        buffer[i++] = '-';
    }

    if (value == 0) {
        console_write_char('0');
        return;
    }

    while (value != 0) {
        buffer[i++] = digits[value % base];
        value /= base;
    }

    // If we added a '-' above, it’s in buffer[0], so we print from the end
    for (int j = i - 1; j >= 0; j--) {
        console_write_char(buffer[j]);
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
                    print_number(va_arg(args, int), 10, true); // signed
                    break;
                case 'u':
                    print_number(va_arg(args, unsigned int), 10, false); // unsigned
                    break;
                case 'x':
                    print_number(va_arg(args, unsigned int), 16, false); // hex
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