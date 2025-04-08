#include "terminal/print.h"
#include "terminal/clear.h"

static char* main_menu =
    "---- MAIN MENU ----\n"
    "1. About Should Have Chosen Windows\n"
    ;

void print_main_menu(void){
    clearTerminal();
    reset_cursor();
    printf("%s", main_menu);   
}