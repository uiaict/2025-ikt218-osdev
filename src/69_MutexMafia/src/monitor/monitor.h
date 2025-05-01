#ifndef MONITOR_H
#define MONITOR_H

#include <libc/stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern volatile char *video_memory;
extern uint16_t cursor;
 extern uint8_t terminal_row;
 extern uint8_t terminal_column;

void scroll();
void clear_screen();
void print_menu();
void move_cursor();
void init_monitor();
void draw_char_at(int x, int y, char c, uint8_t color);




#endif