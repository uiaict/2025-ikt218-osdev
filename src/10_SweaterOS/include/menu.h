#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "snake.h"  // Include the snake game header

// Viser hovedmenyen med alle tilgjengelige valg
void show_menu(void);

// Viser instruksjoner for piano keyboard
void show_piano_instructions(void);

// Håndterer piano keyboard input og spiller noter
void handle_piano_keyboard(void);

// Håndterer brukerens menyvalg
void handle_menu_choice(char choice);

// Kjører menyløkken
void run_menu_loop(void);

#endif // MENU_H 