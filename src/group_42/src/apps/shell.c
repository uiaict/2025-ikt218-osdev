#include "shell.h"
#include "../system.h"
#include "../print.h"

bool shell_active = false;

void shell_init(){
    shell_active = true;
    // Clear the screen

    for(int i = 0; i < SCREEN_HEIGHT; i++){
        clear_line(i);
    }
    cursorPositionX_ = 0;
    cursorPositionY_ = 0;

    cursor_enable(0,0);
    update_cursor(0,1);
}

void shell_input(char character){
    if(character != '\n' && character != 0x08){
    volatile char *video = cursorPosToAddress(cursorPositionX_, cursorPositionY_);
    *video = character;
    video++;
    *video = VIDEO_WHITE;

    incrementCursorPosition();
    update_cursor(cursorPositionX_,cursorPositionY_+1);
    } else if (character == 0x08){
        update_cursor(cursorPositionX_-1,cursorPositionY_+1);
        if(cursorPositionX_ != 0){
            cursorPositionX_--;
        }

        volatile char *video = cursorPosToAddress(cursorPositionX_, cursorPositionY_);
        *video = 0;
        video++;
        *video = VIDEO_WHITE;
    }
}