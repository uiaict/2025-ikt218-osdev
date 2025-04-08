#include "libc/monitor.h"

 volatile uint16_t* video_memory = (volatile uint16_t*)0xB8000;
 
 uint8_t cursor_x = 0;
 uint8_t cursor_y = 0;
 
 #define VGA_WIDTH  80
 #define VGA_HEIGHT 25
 #define WHITE_ON_BLACK 0x0F
 
void monitor_put(char c){
    video_memory[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;

    cursor_x++;
    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }
}


void monitor_write(const char *string ){
    uint8_t colour = 0x07;
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 ) // goes threw all char values in the "string"
    {
        *video++ = *string++;
        *video++ = colour;
    }
}