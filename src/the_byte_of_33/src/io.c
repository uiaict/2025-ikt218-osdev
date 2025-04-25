#include <libc/stddef.h>
#include <libc/stdint.h>
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static uint8_t row = 0;
static uint8_t col = 0;
static uint8_t colour = 0x07;

void set_color(uint8_t c) { colour = c; }

static void put_at(char c, uint8_t r, uint8_t ccol) {
    VGA[r * VGA_WIDTH + ccol] = ((uint16_t)colour << 8) | (uint8_t)c;
}

void putchar(char c) {
    if (c == '\n') {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;
        return;
    }
    put_at(c, row, col);
    if (++col == VGA_WIDTH) {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;
    }
}

void puts(const char *s) {
    while (*s) putchar(*s++);
}

void print_number(uint32_t num) {
    if (num == 0) {
        putchar('0');
        return;
    }
    char buf[32];
    int i = 0;
    while (num) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i--) putchar(buf[i]);
}

void printf(const char* fmt, uint32_t arg) {
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1) == 'd') {
            print_number(arg);
            fmt += 2;
        } else {
            putchar(*fmt);
            fmt++;
        }
    }
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}