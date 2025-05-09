#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "terminal.h"
#include "pit.h"

// Define terminal dimensions for convenience
#define TERMINAL_WIDTH VGA_WIDTH
#define TERMINAL_HEIGHT VGA_HEIGHT

// Function to display the loading screen
void display_loading_screen(void);

// Function to calculate string length (needed for centering text)
size_t terminal_strlen(const char* str);

#endif // LOADING_SCREEN_H
