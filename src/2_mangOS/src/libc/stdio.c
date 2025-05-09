#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/terminal.h"
#include "libc/stdarg.h"
#include "libc/stdlib.h"

static uint16_t cursor_pos = 0;

int putchar(int ic)
{
    char c = (char)ic;

    terminal_put(c);

    return ic;
}

bool print(const char *data, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        putchar(data[i]);
    }
    return true;
}

int printf(const char *__restrict__ format, ...)
{
    va_list args;
    va_start(args, format);

    const char *str;
    int d;
    char buf[32];
    size_t i = 0;

    while (format[i])
    {
        if (format[i] == '%' && format[i + 1])
        {
            i++;
            switch (format[i])
            {
            case 's':
                str = va_arg(args, const char *);
                print(str, strlen(str));
                break;
            case 'd':
                d = va_arg(args, int);
                itoa(d, buf, 10);
                print(buf, strlen(buf));
                break;
            case 'x':
                d = va_arg(args, int);
                itoa(d, buf, 16);
                print(buf, strlen(buf));
                break;
            default:
                putchar('%');
                putchar(format[i]);
                break;
            }
        }
        else
        {
            putchar(format[i]);
        }
        i++;
    }

    va_end(args);
    return i;
}