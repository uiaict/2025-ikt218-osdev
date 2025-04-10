#include "libc/stdio.h"
#include "terminal.h"
#include "libc/stdarg.h"
#include "libc/string.h"

int putchar(int ic) {
    terminal_put_char((char) ic);
    return ic;
}

bool print(const char* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (putchar(data[i]) == -1) return false;
    }
    return true;
}

static void int_to_str(int value, char* buffer) {
    char temp[16];
    int i = 0;
    bool is_negative = false;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = true;
        value = -value;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    int j = 0;
    if (is_negative) buffer[j++] = '-';
    while (i > 0) buffer[j++] = temp[--i];
    buffer[j] = '\0';
}

static void uint_to_str(unsigned int value, char* buffer) {
    char temp[16];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = 0; j < i; ++j)
        buffer[j] = temp[i - j - 1];
    buffer[i] = '\0';
}

static void int_to_hex(unsigned int value, char* buffer) {
    const char* hex_chars = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {
        buffer[9 - i] = hex_chars[value & 0xF];
        value >>= 4;
    }

    buffer[10] = '\0';
}

int printf(const char* __restrict__ format, ...) {
    va_list args;
    va_start(args, format);

    int written = 0;
    char ch;

    while ((ch = *format++) != '\0') {
        if (ch != '%') {
            putchar(ch);
            written++;
        } else {
            ch = *format++;
            char buffer[32];

            switch (ch) {
                case 'd':
                    int_to_str(va_arg(args, int), buffer);
                    print(buffer, strlen(buffer));
                    written += strlen(buffer);
                    break;

                case 'u':
                    uint_to_str(va_arg(args, unsigned int), buffer);
                    print(buffer, strlen(buffer));
                    written += strlen(buffer);
                    break;

                case 'x':
                    int_to_hex(va_arg(args, unsigned int), buffer);
                    print(buffer, strlen(buffer));
                    written += strlen(buffer);
                    break;

                case 'c':
                    putchar((char)va_arg(args, int));
                    written++;
                    break;

                case 's': {
                    const char* str = va_arg(args, const char*);
                    print(str, strlen(str));
                    written += strlen(str);
                    break;
                }

                default:
                    putchar('%');
                    putchar(ch);
                    written += 2;
                    break;
            }
        }
    }

    va_end(args);
    return written;
}
