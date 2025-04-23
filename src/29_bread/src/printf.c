#include <libc/stdint.h>
#include "putchar.h"
#include "libc/stdarg.h"
#include "print.h"

int print_string(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        putchar(str[i]);
        i++;
    }
    return i;
}

int print_int(int num) {
    char str[32];
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        str[i++] = '0';
    } else {
        if (num < 0) {
            is_negative = 1;
            num = -num;
        }

        char temp[32];
        int j = 0;

        while (num > 0) {
            temp[j++] = num % 10 + '0';
            num /= 10;
        }

        if (is_negative) {
            str[i++] = '-';
        }

        while (j > 0) {
            str[i++] = temp[--j];
        }
    }

    str[i] = '\0';
    return print_string(str);
}

// Implementation of printf function
int printf(const char* format, ...) {
    int printed_chars = 0;
    va_list args;
    va_start(args, format);
    
    int i = 0;
    while (format[i] != '\0') {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's': {  // string specifier
                    const char* str_arg = va_arg(args, const char*);
                    printed_chars += print_string(str_arg);
                    break;
                }
                case 'd': {  // integer specifier
                    int int_arg = va_arg(args, int);
                    printed_chars += print_int(int_arg);
                    break;
                }
                case '%': {  // percent sign
                    putchar('%');
                    printed_chars++;
                    break;
                }
                default: {  // Unknown format specifier, print as is
                    putchar('%');
                    putchar(format[i]);
                    printed_chars += 2;
                    break;
                }
            }
        } else {
            putchar(format[i]);
            printed_chars++;
        }
        i++;
    }

    va_end(args);
    return printed_chars;
}