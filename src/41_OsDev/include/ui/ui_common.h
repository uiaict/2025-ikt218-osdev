#ifndef UI_COMMON_H
#define UI_COMMON_H

#include <libc/stdint.h>
#include <libc/stddef.h>

////////////////////////////////////////
// UI Utility Functions
////////////////////////////////////////

// Clear the terminal screen
void clear_screen(void);

// Return the length of a null-terminated string
size_t terminal_strlen(const char* str);

////////////////////////////////////////
// External Terminal Interface
////////////////////////////////////////

extern void terminal_setcursor(int x, int y);
extern void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
extern uint8_t terminal_getcolor(void);
extern void terminal_setcolor(uint8_t color);
extern void terminal_write(const char* str);

#endif // UI_COMMON_H
