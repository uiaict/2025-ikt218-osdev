//monitor.h
#pragma once
#include "libc/stdarg.h"
#include "libc/stddef.h" 
#include "libc/stdint.h"



// We need to include stddef.h for size_t



// This is the main object for the monitor
typedef struct {
    // The cursor position vertically
    int cursor_row;
    // The cursor position horizontally
    int cursor_col;
    // A pointer to the start of VGA text mode memory (0xb8000).
    volatile char* video_memory;
    // The default color attribute used when writing characters.
    unsigned char color;
} monitor_t;

// This will initialize the monitor
void monitor_init();
// output a character to the monitor
void monitor_put_char(char c);
// output a string to the monitor
void monitor_write(const char* str);
// Backspace function
void monitor_backspace();
// enter function
void monitor_enter();
// Scroll down function
void monitor_scroll_down();
// Scroll up function
void monitor_scroll_up();
// Clear the monitor
void monitor_clear();
void monitor_write_dec(int num);