#include <libc/terminal.h>
#include <libc/io.h>

// Pointer til video-minnet (VGA)
volatile uint16_t *video_memory = (uint16_t *)0xB8000;
int cursor_x = 0, cursor_y = 0;

// Skriv én karakter til skjermen
void terminal_putc(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else
    {
        video_memory[cursor_y * 80 + cursor_x] = (0x0F << 8) | c;
        cursor_x++;
    }

    move_cursor(); // <--- Legg til denne linjen
}

// Skriv en hel streng til skjermen
void terminal_write(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        terminal_putc(str[i]);
    }
}

void move_cursor()
{
    uint16_t pos = cursor_y * 80 + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
