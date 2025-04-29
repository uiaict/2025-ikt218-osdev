#include "libc/stdint.h"

#include "../monitor/monitor.h"
#include "../io/keyboard.h"
#include "../io/printf.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};



size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;



void print_menu(){
    clear_screen();
    mafiaPrint("Welcome to Mafia! What do you want to do today?\n");
    mafiaPrint("1. Print Hello World\n");
    mafiaPrint("2. Print memory Layout\n");
    mafiaPrint("3. Allocate some memory\n");
    mafiaPrint("4. Play Mafia Bird\n");
    mafiaPrint("5. Exit\n");
}


void init_monitor(){
    terminal_row = 0;
    terminal_column = 0;
    terminal_buffer = (uint16_t*)video_memory;
    terminal_color = VGA_COLOR_GREEN;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = terminal_color; 
    }
    
}

void clear_screen(){
    terminal_row = 0;
    terminal_column = 0;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
    }
}

void scroll(){
    if (terminal_row >= SCREEN_HEIGHT){
        for (int i = 0; i < (SCREEN_HEIGHT-1)* SCREEN_WIDTH *2; i++){
            video_memory[i] = video_memory[i+SCREEN_WIDTH*2];
        }
        for (int i = (SCREEN_HEIGHT-1)* SCREEN_WIDTH *2; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i++){
            video_memory[i] = ' ';
            video_memory[i+1] = 0x07;
        }
        terminal_row = SCREEN_HEIGHT - 1;
    }
}
void move_cursor(){
    uint16_t position = terminal_row * SCREEN_WIDTH + terminal_column;
    outPortB(0x3D4, 14);
    outPortB(0x3D5, position >> 8);
    outPortB(0x3D4, 15);
    outPortB(0x3D5, position & 0xFF);
}
