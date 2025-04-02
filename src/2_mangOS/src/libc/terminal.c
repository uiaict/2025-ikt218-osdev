#include "libc/terminal.h"

#define VIDEO_MEMORY (uint16_t *)0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

static uint16_t *const video_memory = VIDEO_MEMORY;
static uint8_t terminal_color;
static uint16_t terminal_row;
static uint16_t terminal_column;

static uint16_t make_vgaentry(char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

void terminal_initialize()
{
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_row = 0;
    terminal_column = 0;
    for (size_t y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (size_t x = 0; x < SCREEN_WIDTH; x++)
        {
            const size_t index = y * SCREEN_WIDTH + x;
            video_memory[index] = make_vgaentry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_column = 0;
    }
    else
    {
        const size_t index = terminal_row * SCREEN_WIDTH + terminal_column;
        video_memory[index] = make_vgaentry(c, terminal_color);
        terminal_column++;
        if (terminal_column == SCREEN_WIDTH)
        {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row == SCREEN_HEIGHT)
    {
        // TODO implement scrolling here
        terminal_row = 0;
    }
}

void terminal_writestring(const char *str)
{
    while (*str)
    {
        terminal_putchar(*str++);
    }
}