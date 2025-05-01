#include "screen.h"
#include "isr.h"
#include "idt.h"

// Cursor state
static int cursor_x = 0;
static int cursor_y = 0;

// Color: fg on bg
static unsigned char current_color = (COLOR_WHITE << 4) | COLOR_BLACK;

#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5

// Update hardware cursor
static void update_cursor(void) {
    unsigned short position = cursor_y * SCREEN_WIDTH + cursor_x;
    outb(VGA_CTRL_REGISTER, 14);
    outb(VGA_DATA_REGISTER, position >> 8);
    outb(VGA_CTRL_REGISTER, 15);
    outb(VGA_DATA_REGISTER, position & 0xFF);
}

// Set cursor position
void set_cursor_pos(int x, int y) {
    cursor_x = (x >= SCREEN_WIDTH) ? SCREEN_WIDTH - 1 : x;
    cursor_y = (y >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - 1 : y;
    update_cursor();
}

// Set foreground and background color
void set_text_color(unsigned char fg, unsigned char bg) {
    current_color = (bg << 4) | (fg & 0x0F);
}

// Clear screen with current color
void clear_screen(void) {
    unsigned short blank = ' ' | (current_color << 8);
    unsigned short *vga_buffer = (unsigned short *)VGA_TEXT_BUFFER;

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

// Scroll up by one line
void scroll_screen(void) {
    unsigned short *vga_buffer = (unsigned short *)VGA_TEXT_BUFFER;
    unsigned short blank = ' ' | (current_color << 8);

    // Copy all lines up
    for (int i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + SCREEN_WIDTH];
    }

    // Clear last line
    for (int i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }

    cursor_y = SCREEN_HEIGHT - 1;
}

// Print single character
void print_char(char c) {
    unsigned short *vga_buffer = (unsigned short *)VGA_TEXT_BUFFER;

    switch (c) {
        case '\n':
            cursor_x = 0;
            cursor_y++;
            break;
        case '\r':
            cursor_x = 0;
            break;
        case '\t':
            cursor_x = (cursor_x + 8) & ~(8 - 1);
            break;
        case '\b':
            if (cursor_x > 0) {
                cursor_x--;
                vga_buffer[cursor_y * SCREEN_WIDTH + cursor_x] = ' ' | (current_color << 8);
            }
            break;
        default:
            vga_buffer[cursor_y * SCREEN_WIDTH + cursor_x] = c | (current_color << 8);
            cursor_x++;
            break;
    }

    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= SCREEN_HEIGHT) {
        scroll_screen();
    }

    update_cursor();
}

// Print string
void print_string(const char *str) {
    while (*str) {
        print_char(*str++);
    }
}

// Print integer
void print_int(int n) {
    char buffer[16];
    int i = 0;

    if (n < 0) {
        print_char('-');
        n = -n;
    }

    if (n == 0) {
        print_char('0');
        return;
    }

    while (n > 0) {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i > 0) {
        print_char(buffer[--i]);
    }
}

// Initialize screen
void init_screen(void) {
    clear_screen();
    update_cursor();
}
