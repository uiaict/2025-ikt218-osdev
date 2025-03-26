#include "print.h"



void print_int(int n) {
    if (n < 0) {
        putchar('-');
        n = -n;
    }
    if (n / 10)
        print_int(n / 10);
    putchar(n % 10 + '0');
}

void print_string(const char* s) {
    while (*s) {
        putchar(*s++);
    }
}


void my_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    const char* s = va_arg(args, const char*);
                    print_string(s);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    print_int(d);
                    break;
                }
                case 'c': {
                    // Cast til char siden va_arg krever minimum størrelse på int
                    char c = (char) va_arg(args, int);
                    putchar(c);
                    break;
                }
                default: {
                    putchar('%');
                    putchar(*format);
                    break;
                }
            }
        } else {
            putchar(*format);
        }
        format++;
    }

    va_end(args);
}
