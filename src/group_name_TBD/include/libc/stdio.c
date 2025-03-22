#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"


size_t terminal_width = 80; // Not const as to allow possible resize in the future
size_t terminal_height = 25;
uint8_t terminal_color = 0x0f; // default white text black background

size_t cursor_xpos = 0;
size_t cursor_ypos = 0;

char *video_memory = (char*)0x00b8000;

void set_vga_color(enum vga_color txt_color, enum vga_color bg_color){
    terminal_color = txt_color | bg_color << 4;
}

void printf(const char *string){
    
    for (size_t i = 0; i < strlen(string); i++) {
        
        if (string[i] >= 0 && string[i] <= 31){
            
            ctrlchar(string[i]);
            continue;
        } else {
            
            putchar_at(&string[i], cursor_xpos, cursor_ypos);
            cursor_xpos++;
            verify_cursor_pos();
        }
    }
}

void putchar_at(const char *c, size_t x, size_t y){
    
    const size_t index = y * terminal_width + x;
    video_memory[index*2] = *c;
    video_memory[index*2 +1] = terminal_color;
}



void verify_cursor_pos(){
    // Fix cursor if OoB
    
    if (cursor_xpos > terminal_width){
        cursor_xpos = 0;
        cursor_ypos++;
    }
    if (cursor_ypos > terminal_height){
        cursor_ypos = 0;
    }
}

void ctrlchar(int c){
    // \r\n nedded for regular std::newline
    // Might change to only \n later
    
    switch (c){
        case 10: // \n: Newline, does only LF
            // cursor_xpos = 0;
            cursor_ypos++;
            verify_cursor_pos();
            break;
            
        case 13: // \r: Return, does only CR
        cursor_xpos = 0;
            break;
        
        default:
            break;
    }
}

