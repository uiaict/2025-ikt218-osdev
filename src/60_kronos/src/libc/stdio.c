#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "drivers/terminal.h"

int printf(const char* __restrict__ format, ...) {

    va_list args;
    va_start(args, format);

    int cur_arg = 0;


    while (*format != '\0') {

        if (*format != '%') {
            terminal_write(DEFAULT_COLOR, (const char[]){*format, '\0'});
            format++;
            continue;
        }

        format++;
        switch (*format) {
        case 'd':
        case 'i': {
            char str[32];
            int32_t val = va_arg(args, int32_t);
            itoa(val, str, 10);
            terminal_write(DEFAULT_COLOR, str);
        }
            break;
        case 'u': {
            char str[32];
            uint32_t val = va_arg(args, uint32_t);
            itoa(val, str, 10);
            terminal_write(DEFAULT_COLOR, str);
        }
            break;
        case 'f': {
            char str[32];
            float val = va_arg(args, double);
            int prec = 6;
            if (*(format + 1) == '.' && *(format + 2) >= '0' && *(format + 2) <= '9') {
                prec = (int) (*(format + 2) - '0');  // -'0' for ASCII to dec
                format += 2;
            }
            ftoa(val, str, prec);
            terminal_write(DEFAULT_COLOR, str);
        }
            break;
        case 'c': {
            char c = va_arg(args, int);
            terminal_write(DEFAULT_COLOR, (const char[]){c, '\0'});
            
        }
            break;
        case 's': {
            char *str = va_arg(args, char*);
            terminal_write(DEFAULT_COLOR, str);
        }
            break;
        case 'x': {
            char str[32];
            uint32_t val = va_arg(args, uint32_t);
            itoa(val, str, 16);
            terminal_write(DEFAULT_COLOR, str);
        }
        default:
            break;
        }
        format++;
        cur_arg++;
    }

    terminal_write(WHITE, format);
    return 0;
}