#include "../system.h"
#include "../print.h"

void shell_init(){
    // Clear the screen

    for(int i = 0; i < SCREEN_HEIGHT; i++){
        clear_line(i);
    }

    cursor_enable(0,0);
    update_cursor(0,1);
}


void shell_draw(){}

void shell_input(char character){}