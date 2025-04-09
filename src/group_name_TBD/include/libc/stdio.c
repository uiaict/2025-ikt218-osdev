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


void print(const unsigned char *string){
    
    for (size_t i = 0; i < strlen(string); i++) {
        putchar((int)string[i]);        
    }
}

int putchar(const int c){
    
    if (c > 255 || c < 0){
        return false;
    }
    
    if (c < 32){
        ctrlchar(c);
        return true;
    }
    
    const size_t index = cursor_ypos * terminal_width + cursor_xpos;
    video_memory[index*2] = (unsigned char)c;
    video_memory[index*2 +1] = terminal_color;
    
    cursor_xpos++;
    verify_cursor_pos();
    
    return true;
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
    
    switch (c){
        case '\n':
            cursor_ypos++;
            break;
            
        case '\r':
            cursor_xpos = 0;
            break;
            
        case '\t':
            cursor_xpos = (cursor_xpos / 8 + 1) * 8;
            break;
            
        case '\b':
            cursor_xpos--;
            break; 
            
        case '\f':
            cursor_ypos = (cursor_ypos / terminal_height + 1) * terminal_height;
            cursor_xpos = 0;
            break;
            
        case '\a':
            //TODO: BEEP
            break;
        
        default:
            break;
    }
    verify_cursor_pos();
}

