#include "stddef.h"
#include "stdint.h"
#include "terminal.h"
#include "libc/conv.h"
#include "libc/string.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

uint8_t get_color(uint8_t fg, uint8_t bg) {
    return (bg << 4) | (fg & 0x0F);
}

uint16_t create_vga_entry(char c, uint8_t color) {
    return (uint16_t)(color << 8 | (uint8_t)c);
}

void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = get_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = VGA_BUFFER;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = create_vga_entry(' ', terminal_color);
        }
    }
}

void scroll() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = (y - 1) * VGA_WIDTH + x;
            terminal_buffer[index] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = create_vga_entry(' ', terminal_color);
    }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = create_vga_entry(c, color);
}

static void move_cursor(size_t x, size_t y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 14);
    outb(0x3D5, (uint8_t)(pos >> 8));
    outb(0x3D4, 15);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    if (terminal_row >= VGA_HEIGHT) {
        scroll();
        terminal_row = VGA_HEIGHT - 1;
    }
    move_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_writeint(int32_t value) {
    char buffer[32];
    itoa(value, buffer);
    terminal_writestring(buffer);
}

void terminal_writeuint(uint32_t value) {
    char buffer[32];
    uitoa(value, buffer);
    terminal_writestring(buffer);
}

void terminal_setcolor(uint8_t fg, uint8_t bg) {
    terminal_color = get_color(fg, bg);
}

void terminal_setcursor(size_t x, size_t y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        terminal_column = x;
        terminal_row = y;
    }
    move_cursor(terminal_column, terminal_row);
}

void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = create_vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

void terminal_hello() {
    terminal_writestring("Hello, World!\n> ");
}

size_t terminal_get_column(void) {
    return terminal_column;
}

size_t terminal_get_row(void) {
    return terminal_row;
}

void terminal_putchar_color(char c, uint8_t color) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
    } else {
        terminal_putentryat(c, color, terminal_column, terminal_row);
        terminal_column++;
    }
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    if (terminal_row >= VGA_HEIGHT) {
        scroll();
        terminal_row = VGA_HEIGHT - 1;
    }
    move_cursor(terminal_column, terminal_row);
}

void terminal_write_color(const char* data, size_t size, uint8_t color) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar_color(data[i], color);
    }
}

void terminal_writestring_color(const char* data, uint8_t color) {
    terminal_write_color(data, strlen(data), color);
}

void terminal_writeint_color(int32_t value, uint8_t color) {
    char buffer[32];
    itoa(value, buffer);
    terminal_writestring_color(buffer, color);
}

void terminal_writeuint_color(uint32_t value, uint8_t color) {
    char buffer[32];
    uitoa(value, buffer);
    terminal_writestring_color(buffer, color);
}

void clear_input_line() {
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        terminal_putentryat(' ', terminal_color, i, terminal_row);
    }
    terminal_column = 0;
    move_cursor(terminal_column, terminal_row);
    terminal_writestring("> ");
}