#include "monitor.h"

uint8_t terminal_color;
uint16_t *terminal_buffer;
uint16_t cursor;
uint8_t terminal_row;
uint8_t terminal_column;

void print_menu() {
    Print("What do you want??\n");
    Print("==============================\n");
    Print("1. Print Hello World\n");
    Print("2. Print memory Layout\n");
    Print("3. Allocate some memory\n");
    Print("4. Play music\n");
    Print("5. Text editor\n");
    Print("6. Clear screen and print menu\n");
    Print("==============================\n");
}

void init_monitor() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_buffer = (uint16_t *)video_memory;
    terminal_color = VGA_COLOR_GREEN;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = terminal_color;
    }
}

void clear_screen() {
    terminal_row = 0;
    terminal_column = 0;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
    }
}

void scroll() {

    for (int i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH * 2; i++) {
        video_memory[i] = video_memory[i + SCREEN_WIDTH * 2];
    }
    for (int i = (SCREEN_HEIGHT - 1) * (SCREEN_WIDTH * 2); i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07;
    }
    cursor -= SCREEN_WIDTH * 2;
    if (terminal_row > 0)
        terminal_row--;
    terminal_column = 0;

    move_cursor();
}

void move_cursor() {
    uint16_t position = terminal_row * SCREEN_WIDTH + terminal_column;
    outPortB(0x3D4, 14);
    outPortB(0x3D5, position >> 8);
    outPortB(0x3D4, 15);
    outPortB(0x3D5, position & 0xFF);
}

void draw_char_at(int x, int y, char c, uint8_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
        return;
    int index = (y * SCREEN_WIDTH + x) * 2;
    video_memory[index] = c;
    video_memory[index + 1] = color;
}