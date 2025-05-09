#include "i386/monitor.h"
#include "libc/system.h"
int lastrowlen;

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
static void move_cursor()
{
    // The screen is 80 characters wide...
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

// Moves cursor according to input
void move_cursor_direction(int move_x, int move_y)
{
    terminal_column += move_x;
    terminal_row += move_y;
    move_cursor();
}

void monitor_initialize(void)
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

void monitor_setcolor(uint8_t color)
{
    terminal_color = color;
}

void monitor_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void _monitor_put(char c)
{
    // Deal with special character behavior
    switch (c)
    {
    case '\n':
        lastrowlen = terminal_column;
        terminal_column = 0;
        terminal_row++;
        scroll();
        return;
        break;
    case '\b':
        if (terminal_column == 0)
        {
            terminal_column = lastrowlen;
            monitor_putentryat(' ', terminal_color, terminal_column, --terminal_row);
        }
        else
            monitor_putentryat(' ', terminal_color, --terminal_column, terminal_row);
        return;
        break;
    default:
        break;
    }

    monitor_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        lastrowlen = VGA_WIDTH - 1;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void monitor_put(char c)
{
    _monitor_put(c);
    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}

void monitor_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        _monitor_put(data[i]);

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();
}

void monitor_writestring(const char *data)
{
    monitor_write(data, strlen(data));
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
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
