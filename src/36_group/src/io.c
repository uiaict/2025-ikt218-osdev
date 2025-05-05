#include "libc/stdio.h" // Use stdio.h instead of io.h for printf declarations
#include <libc/stddef.h>
#include <libc/stdint.h>
#include "io.h"

// VGA definitions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static uint8_t row = 0;
static uint8_t col = 0;
static uint8_t colour = 0x07;

void set_color(uint8_t c) { colour = c; }

static void put_at(char c, uint8_t r, uint8_t ccol)
{
    VGA[r * VGA_WIDTH + ccol] = ((uint16_t)colour << 8) | (uint8_t)c;
}

int putchar(int ic)
{
    char c = (char)ic;
    if (c == '\n')
    {
        col = 0;
        if (++row == VGA_HEIGHT)
            row = 0;
        return ic;
    }
    put_at(c, row, col);
    if (++col == VGA_WIDTH)
    {
        col = 0;
        if (++row == VGA_HEIGHT)
            row = 0;
    }
    return ic;
}

void puts(const char *s)
{
    while (*s)
        putchar(*s++);
}

void clear_screen(void)
{
    for (uint8_t r = 0; r < VGA_HEIGHT; r++)
    {
        for (uint8_t ccol = 0; ccol < VGA_WIDTH; ccol++)
        {
            put_at(' ', r, ccol);
        }
    }
    row = 0;
    col = 0;
}

void print_number(uint32_t num)
{
    if (num == 0)
    {
        putchar('0');
        return;
    }
    char buf[32];
    int i = 0;
    while (num)
    {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i--)
        putchar(buf[i]);
}

void print_string(const char *str)
{
    while (*str)
        putchar(*str++);
}

void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        if (*fmt == '%' && *(fmt + 1))
        {
            fmt++;
            if (*fmt == 'd')
            {
                uint32_t arg = va_arg(args, uint32_t);
                print_number(arg);
            }
            else if (*fmt == 's')
            {
                char *arg = va_arg(args, char *);
                print_string(arg);
            }
            else
            {
                putchar(*fmt);
            }
        }
        else
        {
            putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

void print_dec(uint32_t value)
{
    if (value == 0)
    {
        putchar('0');
        return;
    }

    char buf[16];
    int i = 0;
    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--)
    {
        putchar(buf[i]);
    }
}

void print_hex(uint32_t value)
{
    const char *hex_digits = "0123456789ABCDEF";

    putchar('0');
    putchar('x');
    for (int i = 7; i >= 0; i--)
    {
        uint8_t digit = (value >> (i * 4)) & 0xF;
        putchar(hex_digits[digit]);
    }
}

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
