#include "terminal.h"
#include <stdarg.h>
#include <stdbool.h>
#include "string.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* const vga_buffer = (uint16_t*) VGA_ADDRESS;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x07; // Light grey on black

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Ekranı kaydırma fonksiyonu
static void terminal_scroll() {
    // Satırları bir yukarı kaydır
    memmove(vga_buffer, vga_buffer + VGA_WIDTH, (VGA_HEIGHT-1)*VGA_WIDTH*2);
    
    // Son satırı temizle
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT-1)*VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }
    terminal_row = VGA_HEIGHT - 1;
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
    // Test için sol üst köşeye bir karakter yaz
    vga_buffer[0] = vga_entry('X', 0x0A); // Yeşil 'X'
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
        }
        return;
    }

    vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
    
    if (++terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// Kısaltılmış printf (sadece temel formatlar)
void terminal_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    terminal_write(str, strlen(str));
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 10);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    terminal_putchar(c);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[32];
                    itoa(num, buffer, 16);
                    terminal_write(buffer, strlen(buffer));
                    break;
                }
                default:
                    terminal_putchar(*fmt);
            }
        } else {
            terminal_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}