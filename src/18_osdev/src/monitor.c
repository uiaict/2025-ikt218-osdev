#include "../include/libc/monitor.h"
#include "libc/common.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 15

static uint16_t* const video_memory = (uint16_t*) 0xB8000;
static size_t cursor_row = 0;
static size_t cursor_col = 0;


void move_cursor() {
    uint16_t cursor_location = cursor_row * VGA_WIDTH + cursor_col;

    outb(0x3D4, 14); // High byte
    outb(0x3D5, cursor_location >> 8);
    outb(0x3D4, 15); // Low byte
    outb(0x3D5, cursor_location & 0xFF);
}

static void scroll_if_needed() {
    if (cursor_row >= VGA_HEIGHT) {
        for (size_t row = 1; row < VGA_HEIGHT; row++) {
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                video_memory[(row - 1) * VGA_WIDTH + col] = video_memory[row * VGA_WIDTH + col];
            }
        }

        for (size_t col = 0; col < VGA_WIDTH; col++) {
            video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = ' ' | (VGA_COLOR << 8);
        }

        cursor_row = VGA_HEIGHT - 1;
    }
}

void monitor_initialize(void) {
    monitor_clear();
}

void monitor_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            video_memory[y * VGA_WIDTH + x] = ' ' | (VGA_COLOR << 8);
        }
    }
    cursor_row = 0;
    cursor_col = 0;
}

void monitor_put(char c, int color) {
    if (c == 0x08 && cursor_col > 0) { // Backspace
        cursor_col--;
    } else if (c == 0x09) { // Tab
        cursor_col = (cursor_col + 8) & ~(8 - 1);
    } else if (c == '\r') { // Carriage return
        cursor_col = 0;
    } else if (c == '\n') { // Newline
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
    } else if (c >= ' ') { // Printable character
        video_memory[cursor_row * VGA_WIDTH + cursor_col] = (uint16_t)c | (color << 8);
        cursor_col++;
    }

    // Wrap to new line if we go past the screen width
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        scroll_if_needed();
    }

    move_cursor(); // Update hardware cursor
}

void monitor_remove_char(){
    if(cursor_col==0 && cursor_row==0) return;

    if (cursor_col==0){ // go one column up
        cursor_row--;
        cursor_col = VGA_WIDTH - 1;
    } else { // go back
        cursor_col--;
    }

    uint16_t* location = (uint16_t*)video_memory + (cursor_row * VGA_WIDTH + cursor_col);
    *location = ' ' ;// Clear the character
    move_cursor(); // Update hardware cursor

}

void monitor_write(const char* str) {
    int i = 0;
    while (str[i]) {
        monitor_put(str[i++], 15);
    }
}

void monitor_write_color(int color, const char* str){
    int i = 0;
    while (str[i])
    {
        monitor_put(str[i++], color);
    }
    
}

void monitor_write_dec(uint32_t n) {
    if (n == 0) {
        monitor_put('0',15);
        return;
    }

    char buf[10];
    size_t i = 0;

    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i--) {
        monitor_put(buf[i],15);
    }
}

void monitor_write_hex(uint32_t num) {
    char hex_digits[] = "0123456789ABCDEF";
    char buffer[11] = "0x00000000";
    for (int i = 9; i >= 2; --i) {
        buffer[i] = hex_digits[num & 0xF];
        num >>= 4;
    }
    monitor_write(buffer);
}
