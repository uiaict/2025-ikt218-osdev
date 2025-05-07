// terminal.c
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "terminal.h"
#include "port_io.h"         /* outb / inb for VGA cursor ports  */

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

/* current cursor position in characters (row, col) */
static size_t terminal_row    = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F;  /* light‑gray on black */

/* -------------  Low‑level HW‑cursor helpers ------------- */
#define VGA_PORT_CTRL  0x3D4
#define VGA_PORT_DATA  0x3D5

static inline void vga_set_hw_cursor(uint16_t pos)
{
    outb(VGA_PORT_CTRL, 0x0F);
    outb(VGA_PORT_DATA, pos & 0xFF);
    outb(VGA_PORT_CTRL, 0x0E);
    outb(VGA_PORT_DATA, (pos >> 8) & 0xFF);
}

static inline void terminal_update_cursor(void)
{
    vga_set_hw_cursor((uint16_t)(terminal_row * VGA_WIDTH + terminal_column));
}
/* -------------------------------------------------------- */

void terminal_initialize(void)
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        VGA_MEMORY[i] = (uint16_t)terminal_color << 8 | ' ';

    terminal_row = terminal_column = 0;
    terminal_update_cursor();
}

void terminal_write(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        char c = str[i];

        /* ---------- newline ---------- */
        if (c == '\n')
        {
            terminal_column = 0;
            ++terminal_row;
        }
        /* ---------- backspace ---------- */
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
        /* ---------- printable ---------- */
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

        /* ---------- scroll ---------- */
        if (terminal_row >= VGA_HEIGHT)
        {
            /* shift rows up */
            for (size_t row = 1; row < VGA_HEIGHT; ++row)
                for (size_t col = 0; col < VGA_WIDTH; ++col)
                    VGA_MEMORY[(row - 1) * VGA_WIDTH + col] =
                        VGA_MEMORY[row * VGA_WIDTH + col];

            /* clear last row */
            for (size_t col = 0; col < VGA_WIDTH; ++col)
                VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
                    (uint16_t)terminal_color << 8 | ' ';

            terminal_row = VGA_HEIGHT - 1;
        }
    }

    /* move hardware cursor to new location */
    terminal_update_cursor();
}
