#ifndef printf_h
#define printf_h

#include <libc/stdint.h>
#include "putchar.h"
#include "libc/stdarg.h"

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

// 
int printf(const char* format, ...) {
    int i = 0;
    va_list args;
    va_start(args, format);
    
    while (format[i] != '\0') {
        if (format[i] == '%') {
            i++;
            if (format[i] == 's') {  // string specifier
                const char* str_arg = va_arg(args, const char*);
                print_string(str_arg);
            }
            else if (format[i] == 'd') {  // integer specifier
                int int_arg = va_arg(args, int);
                print_int(int_arg);
            }
            else if (format[i] == '\n') {  // newline
                putchar('\n');
            }
            else {  // H
                putchar(format[i]);
            }
        }
        else {  // no format specifier
            putchar(format[i]);
        }
        i++;
    }

    va_end(args);
    return i;
}

#endif
