// terminal.c -- Defines functions for writing to the terminal.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "libc/terminal.h"
#include "libc/system.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

uint16_t *video_memory = (uint16_t *)0xB8000;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

// Scrolls the text on the screen up by one line.
static void scroll()
{

    // Get a space character with the default colour attributes.
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if (terminal_row >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0 * 80; i < 24 * 80; i++)
        {
            terminal_buffer[i] = terminal_buffer[i + 80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24 * 80; i < 25 * 80; i++)
        {

            terminal_buffer[i] = blank;
        }
        // The cursor should now be on the last line.
        terminal_row = 24;
    }
}

// Updates the hardware cursor.
void move_cursor()
{
    // The screen is 80 characters wide
    uint16_t pos = terminal_row * 80 + terminal_column;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void move_cursor_to(uint8_t x, uint8_t y)
{
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = video_memory;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(enum vga_color color)
{
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void _terminal_put(char c)
{
    // Deal with special character behavior
    switch (c)
    {
    case '\n':
        terminal_column = 0;
        terminal_row++;
        scroll();
        return;

    case '\b':
        if (terminal_column > 0)
        {
            terminal_column--;
        }
        else if (terminal_row > 0)
        {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
        }

        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        return;

    default:
        break;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_put(char c)
{
    _terminal_put(c);
    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        _terminal_put(data[i]);

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}

void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void terminal_clear()
{
    // Make an attribute byte for the default colours
    uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80 * 25; i++)
    {
        terminal_buffer[i] = blank;
    }

    // Move the hardware cursor back to the start.
    terminal_row = 0;
    terminal_column = 0;
    move_cursor();
}

void terminal_write_hex(uint32_t n)
{
    int32_t tmp;
    char *item = "0x";

    terminal_write(item, strlen(item));

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }

        if (tmp >= 0xA)
        {
            noZeroes = 0;
            terminal_put(tmp - 0xA + 'a');
        }
        else
        {
            noZeroes = 0;
            terminal_put(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        terminal_put(tmp - 0xA + 'a');
    }
    else
    {
        terminal_put(tmp + '0');
    }
}

void terminal_write_dec(uint32_t n)
{

    if (n == 0)
    {
        terminal_put('0');
        return;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2[i--] = c[j++];
    }
    terminal_write(c2, strlen(c2));
}
