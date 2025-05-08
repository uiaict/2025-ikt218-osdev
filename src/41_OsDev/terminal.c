// terminal.c
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "terminal.h"
#include "port_io.h"        

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// VGA color definitions
#define VGA_COLOR_BLACK         0
#define VGA_COLOR_BLUE          1
#define VGA_COLOR_GREEN         2
#define VGA_COLOR_CYAN          3
#define VGA_COLOR_RED           4
#define VGA_COLOR_MAGENTA       5
#define VGA_COLOR_BROWN         6
#define VGA_COLOR_LIGHT_GREY    7
#define VGA_COLOR_DARK_GREY     8
#define VGA_COLOR_LIGHT_BLUE    9
#define VGA_COLOR_LIGHT_GREEN   10
#define VGA_COLOR_LIGHT_CYAN    11
#define VGA_COLOR_LIGHT_RED     12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN   14
#define VGA_COLOR_WHITE         15

static size_t terminal_row    = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F;  // White on black

#define VGA_PORT_CTRL  0x3D4
#define VGA_PORT_DATA  0x3D5

// Function to set the VGA hardware cursor position
static inline void vga_set_hw_cursor(uint16_t pos)
{
    outb(VGA_PORT_CTRL, 0x0F);
    outb(VGA_PORT_DATA, pos & 0xFF);
    outb(VGA_PORT_CTRL, 0x0E);
    outb(VGA_PORT_DATA, (pos >> 8) & 0xFF);
}

// Update cursor position based on terminal_row and terminal_column
static inline void terminal_update_cursor(void)
{
    vga_set_hw_cursor((uint16_t)(terminal_row * VGA_WIDTH + terminal_column));
}

// Initialize the terminal by clearing the screen
void terminal_initialize(void)
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        VGA_MEMORY[i] = (uint16_t)terminal_color << 8 | ' ';

    terminal_row = terminal_column = 0;
    terminal_update_cursor();
}

// Clear the screen and reset cursor to top-left
void terminal_clear(void)
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        VGA_MEMORY[i] = (uint16_t)terminal_color << 8 | ' ';

    terminal_row = terminal_column = 0;
    terminal_update_cursor();
}

// Set terminal color
void terminal_set_color(uint8_t color)
{
    terminal_color = color;
}

// Create a color value from foreground and background colors
uint8_t terminal_make_color(uint8_t fg, uint8_t bg)
{
    return fg | (bg << 4);
}

// Put a single character to the terminal
void terminal_putchar(char c)
{
    char str[2] = { c, '\0' };
    terminal_write(str);
}

// Write a string to the terminal
void terminal_write(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        char c = str[i];

        // Handle newline
        if (c == '\n')
        {
            terminal_column = 0;
            ++terminal_row;
        }
        
        // Handle backspace
        else if (c == '\b')
        {
            if (terminal_column)
                --terminal_column;
            else if (terminal_row)
            {
                --terminal_row;
                terminal_column = VGA_WIDTH - 1;
            }
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] =
                (uint16_t)terminal_color << 8 | ' ';
        }
        
        // Normal character
        else
        {
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] =
                (uint16_t)terminal_color << 8 | c;
            if (++terminal_column >= VGA_WIDTH)
            {
                terminal_column = 0;
                ++terminal_row;
            }
        }

        // Handle scrolling if we reach the bottom of the screen
        if (terminal_row >= VGA_HEIGHT)
        {
            // Move all lines up by one
            for (size_t row = 1; row < VGA_HEIGHT; ++row)
                for (size_t col = 0; col < VGA_WIDTH; ++col)
                    VGA_MEMORY[(row - 1) * VGA_WIDTH + col] =
                        VGA_MEMORY[row * VGA_WIDTH + col];

            // Clear the bottom line
            for (size_t col = 0; col < VGA_WIDTH; ++col)
                VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
                    (uint16_t)terminal_color << 8 | ' ';

            terminal_row = VGA_HEIGHT - 1;
        }
    }

    // Update the hardware cursor
    terminal_update_cursor();
}