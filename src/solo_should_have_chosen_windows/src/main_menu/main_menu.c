#include "terminal/print.h"
#include "terminal/clear.h"

static const char* main_menu =
    "---- MAIN MENU ----\n"
    "1. About Should Have Chosen Windows\n"
    "2. Music Player\n"
    "3. ASCII art board\n"
    ;


void print_main_menu(void){
    clearTerminal();
    printf("%s", main_menu);   
}