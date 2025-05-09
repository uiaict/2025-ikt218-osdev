#include <libc/stdarg.h>
#include <libc/stdio.h>
#include <libc/string.h>

#define VIDEO_MEMORY 0xB8000
#define WIDTH 80
#define HEIGHT 25

int col = 0;
int row = 0;
char color = 0x07; //light gray on black

void clear() {
    char *video = (char*) VIDEO_MEMORY;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = color;
    }
    row = 0;
    col = 0;
}

void scroll_up() {
    char *video = (char*) VIDEO_MEMORY;

    for (int r = 1; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            int from = (r * WIDTH + c) * 2;
            int to = ((r - 1) * WIDTH + c) * 2;
            video[to] = video[from];
            video[to + 1] = video[from + 1];
        }
    }

    for (int c = 0; c < WIDTH; c++) {
        int index = ((HEIGHT - 1) * WIDTH + c) * 2;
        video[index] = ' ';
        video[index + 1] = color;
    }
}

void nl() {
    col = 0;
    row++;
    if (row >= HEIGHT) {
        scroll_up();
        row = HEIGHT - 1;
    }
}


int putchar(int ch) {
    char* video = (char*) VIDEO_MEMORY;

    if (ch == '\n') {
        nl();
    } else if (ch == '\r') {
        col = 0;
    } else {
        if (col >= WIDTH) {
            nl();
        }
        int index = (row * WIDTH + col) * 2;
        video[index] = (char)ch;
        video[index + 1] = color;
        col++;
    }

    return ch;
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[32];
    int written = 0;

    while (*format) {
        if (*format == '%' && *(format + 1) == 'd') {
            int val = va_arg(args, int);
            int_to_str(val, buffer);
            for (char* c = buffer; *c; c++) {
                putchar(*c);
                written++;
            }
            format += 2;
        } else {
            putchar(*format++);
            written++;
        }
    }

    va_end(args);
    return written;
}

void print_hex(uint32_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[11];
    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 8; i++) {
        int nibble = (num >> ((7 - i) * 4)) & 0xF;
        buffer[2 + i] = hex_chars[nibble];
    }

    buffer[10] = '\0';
    printf(buffer);
}
