#include "libc/monitor.h"

 volatile uint16_t* video_memory = (volatile uint16_t*)VGA_ADDR;
 
 uint8_t cursor_x = 0;
 uint8_t cursor_y = 0;
 
void monitor_put_with_color(char c, int x, int y, uint8_t color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) return;

    video_memory[y * VGA_WIDTH + x] = (color << 8) | c;
}
 
void monitor_put(char c){

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
    else{
        video_memory[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;

        cursor_x++;
        if (cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    // Handle scrolling if cursor goes off the bottom
    if (cursor_y >= VGA_HEIGHT) {
        // Scroll up by one line
        for (int y = 1; y < VGA_HEIGHT; y++) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                video_memory[(y-1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
            }
        }

        // Clear last line
        for (int x = 0; x < VGA_WIDTH; x++) {
            video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (WHITE_ON_BLACK << 8) | ' ';
        }

        cursor_y = VGA_HEIGHT - 1;
    }
}


void monitor_write(const char *string) {
    while (*string != 0) {
        monitor_put(*string++);
    }
}

void monitor_backspace() {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }

    video_memory[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | ' ';
}

void monitor_newline() {
    cursor_x = 0;
    cursor_y++;
}

// Print a number in decimal
void monitor_write_dec(uint32_t n) {
    if (n == 0) {
        monitor_write("0");
        return;
    }

    char buf[16];
    int i = 0;

    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i--) {
        char str[2] = { buf[i], 0 };
        monitor_write(str);
    }
}

// Print a number in hexadecimal
void monitor_write_hex(uint32_t n) {
    char hex_chars[] = "0123456789ABCDEF";

    monitor_write("0x");
    for (int i = 7; i >= 0; i--) {
        char c = hex_chars[(n >> (i * 4)) & 0xF];
        char str[2] = { c, 0 };
        monitor_write(str);
    }
}
