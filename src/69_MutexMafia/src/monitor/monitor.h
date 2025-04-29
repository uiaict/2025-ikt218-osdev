#ifndef MONITOR_H
#define MONITOR_H

extern volatile char *video_memory;
extern int cursor;

void scroll();
void clear_screen();
void print_menu();
void move_cursor();
void init_monitor();




#endif