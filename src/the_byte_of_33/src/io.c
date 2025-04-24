#include <libc/stddef.h>
#include <libc/stdint.h>
#include "io.h"

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static uint8_t  row   = 0;
static uint8_t  col   = 0;
static uint8_t  colour = 0x07;      /* light-grey on black */

/* --------------------------------------------------------------------- */
void set_color(uint8_t c) { colour = c; }

/* --------------------------------------------------------------------- */
static void put_at(char c, uint8_t r, uint8_t ccol)
{
    VGA[r * VGA_WIDTH + ccol] = ((uint16_t)colour << 8) | (uint8_t)c;
}

/* --------------------------------------------------------------------- */
void putchar(char c)
{
    if (c == '\n') {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;    /* crude wrap, no scroll */
        return;
    }

    put_at(c, row, col);

    if (++col == VGA_WIDTH) {
        col = 0;
        if (++row == VGA_HEIGHT) row = 0;
    }
}

/* --------------------------------------------------------------------- */
void puts(const char *s)
{
    while (*s) putchar(*s++);
}
