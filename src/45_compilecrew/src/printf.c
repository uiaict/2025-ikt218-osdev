#include "libc/terminal.h"
#include "libc/stdarg.h"

// Convert integer to string (base 10 or 16)
static void itoa(int value, char* str, int base) {
    char digits[] = "0123456789ABCDEF";
    char buffer[32];  // Temporary buffer
    int i = 0, isNegative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    while (value) {
        buffer[i++] = digits[value % base];
        value /= base;
    }

    if (isNegative)
        buffer[i++] = '-';

    buffer[i] = '\0';

    // Reverse the string
    int j = 0;
    while (i > 0) {
        str[j++] = buffer[--i];
    }
    str[j] = '\0';
}

// `printf` function
void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[32];

    while (*format) {
        if (*format == '%') {
            format++; // Move past '%'

            switch (*format) {
                case 'd': // Integer
                    itoa(va_arg(args, int), buffer, 10);
                    terminal_write(buffer);
                    break;
                case 'x': // Hexadecimal
                    itoa(va_arg(args, int), buffer, 16);
                    terminal_write("0x");
                    terminal_write(buffer);
                    break;
                case 's': // String
                    terminal_write(va_arg(args, char*));
                    break;
                case 'c': // Character
                    terminal_putchar((char)va_arg(args, int));
                    break;
                case '%': // Literal '%'
                    terminal_putchar('%');
                    break;
                default:
                    terminal_putchar('%');
                    terminal_putchar(*format);
                    break;
            }
        } else {
            terminal_putchar(*format);
        }
        format++;
    }

    va_end(args);
}
