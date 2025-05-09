#include "printf.h"

volatile char *video_memory = (volatile char *)0xB8000; // minneadresse til VGA tekstbuffer

void putc(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
    } else {
        const size_t index = (terminal_row * SCREEN_WIDTH + terminal_column) * 2;
        video_memory[index] = c;
        video_memory[index + 1] = 0x07;

        terminal_column++;
        if (terminal_column >= SCREEN_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }
    if (terminal_row >= SCREEN_HEIGHT) {
        scroll();
    }
    cursor = (terminal_row * SCREEN_WIDTH + terminal_column) * 2;
    move_cursor();
}

void int_to_string(int num, char *str, int base) {
    int i = 0;
    int isNegative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0) {
        isNegative = 1;
        num = -num;
    } 
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0'; // konverterer til ASCII
        num = num / base;
    }
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        start++;
        end--;
    }
}

void Print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[32];

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;

            switch (format[i]) {
            case 'c': {
                char c = (char)va_arg(args, int);
                putc(c);
                break;
            }

            case 's': {
                char *s = va_arg(args, char *);
                while (*s) {
                    putc(*s);
                    s++;
                }
                break;
            }
            case 'i': {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 10);
                for (int j = 0; buffer[j] != '\0'; j++) {
                    putc(buffer[j]);
                }
                break;
            }
            case 'd': {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 10);
                for (int j = 0; buffer[j] != '\0'; j++) {
                    putc(buffer[j]);
                }
                break;
            }
            case 'x': {
                int num = va_arg(args, int);
                int_to_string(num, buffer, 16);
                for (int j = 0; buffer[j] != '\0'; j++) {
                    putc(buffer[j]);
                }
                break;
            }
            default:
                putc('%');
                putc(format[i]);
                break;
            }
        } else {
            putc(format[i]);
        }
    }
    va_end(args);
}