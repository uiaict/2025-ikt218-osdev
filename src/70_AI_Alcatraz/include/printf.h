#include <stdarg.h>
#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static uint16_t* terminal_buffer = (uint16_t*) VGA_ADDRESS;
static int cursor_x = 0, cursor_y = 0;

void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = (WHITE_ON_BLACK << 8) | ' ';
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        terminal_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        clear_screen();
    }

    move_cursor();
}

void print(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void print_int(int num) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }
    char buffer[10];
    int i = 0;
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num);
    
    while (i--) {
        putchar(buffer[i]);
    }
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c':
                    putchar((char)va_arg(args, int));
                    break;
                case 's':
                    print(va_arg(args, char*));
                    break;
                case 'd':
                    print_int(va_arg(args, int));
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    putchar(*format);
            }
        } else {
            putchar(*format);
        }
        format++;
    }
    
    va_end(args);
}
