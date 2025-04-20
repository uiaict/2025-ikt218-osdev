#include "libc/stdint.h"
#include "arch/i386/console.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define COLOR8_BLACK 0
#define COLOR8_BLUE 1
#define COLOR8_GREEN 2
#define COLOR8_CYAN 3
#define COLOR8_RED 4
#define COLOR8_MAGENTA 5
#define COLOR8_BROWN 6
#define COLOR8_LIGHT_GREY 7
#define COLOR8_DARK_GREY 8
#define COLOR8_LIGHT_BLUE 9
#define COLOR8_LIGHT_GREEN 10
#define COLOR8_LIGHT_CYAN 11
#define COLOR8_LIGHT_RED 12
#define COLOR8_LIGHT_MAGENTA 13
#define COLOR8_LIGHT_BROWN 14
#define COLOR8_WHITE 15


static uint16_t* video_memory = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;
const uint16_t defaultColor = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
uint16_t currentColor = defaultColor;


void console_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (0x07 << 8) | ' ';
    }
    cursor_row = cursor_col = 0;
}

void scroll_up(){
    for (uint16_t y = 0; y < VGA_HEIGHT; y++){
        for (uint16_t x = 0; x < VGA_WIDTH; x++){
            int index = (y-1) * VGA_WIDTH + x; 
            video_memory[index] = video_memory[y * VGA_WIDTH + x];
        }
    }

    for (uint16_t x = 0; x < VGA_WIDTH; x++){
        int index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        video_memory[index] = ' ' | currentColor;
    }
}

void new_line(){
    if (cursor_row < VGA_HEIGHT - 1){
        cursor_row++;
        cursor_col = 0;
    } else {
        scroll_up();
        cursor_col = 0;
    }
}

void console_write_char(char c) {
    switch (c)
    {
    case '\n':
        new_line();
        break;
    case '\r':
        cursor_col = 0;
        break;
    case '\b':
        if (cursor_col == 0 && cursor_row != 0) {
            cursor_row--;
            cursor_col = VGA_WIDTH;
        }
        video_memory[cursor_row * VGA_WIDTH + (--cursor_col)] = ' ' | currentColor;
        break;
    case '\t':
        if (cursor_col == VGA_WIDTH){
            new_line();
        }
        uint16_t tabLen = 4 - (cursor_col % 4);
        while (tabLen != 0){
            int index = cursor_row * VGA_HEIGHT + (cursor_col++);
            video_memory[index] = ' ' | currentColor;
            tabLen--;
        }
        break;
    default:
        int index = cursor_row * VGA_WIDTH + cursor_col;
        video_memory[index] = currentColor | c;
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            new_line();
        }
        break;
    }
}

void console_write(const char* str) {
    while (*str) {
        console_write_char(*str++);
    }
}