#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "snake.h"  // Include the snake game header

// Viser hovedmenyen med alle tilgjengelige valg
void show_menu(void);

// Håndterer brukerens menyvalg
void handle_menu_choice(char choice);

// Kjører menyløkken
void run_menu_loop(void);

#endif // MENU_H 