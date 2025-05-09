#include "kprint.h"
#include "screen.h"  // For screen-related printing

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static int kprint_cursor_pos = 0;

// Helper to convert linear position to screen coordinates
static void update_cursor_pos() {
    int x = kprint_cursor_pos % VGA_WIDTH;
    int y = kprint_cursor_pos / VGA_WIDTH;
    set_cursor_pos(x, y);
}

// Print a string
void kprint(const char *str) {
    while (*str) {
        if (*str == '\n') {
            int line = kprint_cursor_pos / VGA_WIDTH;
            kprint_cursor_pos = (line + 1) * VGA_WIDTH;
        } else if (*str == '\b') {
            if (kprint_cursor_pos > 0) {
                kprint_cursor_pos--;
                set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
                print_char(' ');
                set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
            }
        } else {
            set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
            print_char(*str);
            kprint_cursor_pos++;
        }

        // Scroll if needed
        if (kprint_cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
            scroll_screen();
            kprint_cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
        }

        str++;
    }

    update_cursor_pos();
}

// Print a single character
void kprint_char(char c) {
    if (c == '\n') {
        int line = kprint_cursor_pos / VGA_WIDTH;
        kprint_cursor_pos = (line + 1) * VGA_WIDTH;
    } else if (c == '\b') {
        if (kprint_cursor_pos > 0) {
            kprint_cursor_pos--;
            set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
            print_char(' ');
            set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
        }
    } else {
        set_cursor_pos(kprint_cursor_pos % VGA_WIDTH, kprint_cursor_pos / VGA_WIDTH);
        print_char(c);
        kprint_cursor_pos++;
    }

    // Scroll if needed
    if (kprint_cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        scroll_screen();
        kprint_cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
    }

    update_cursor_pos();
}

// Print a hexadecimal number
void kprint_hex(unsigned long num) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[19] = "0x0000000000000000";

    for (int i = 17; i >= 2; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }

    kprint(buffer);
}

// Print a decimal number
void kprint_dec(unsigned long num) {
    char buffer[21];
    int i = 0;

    if (num == 0) {
        kprint("0");
        return;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i > 0) {
        char c[2] = {buffer[--i], '\0'};
        kprint(c);
    }
}

// Clear the screen and reset kprint position
void kprint_clear(void) {
    clear_screen();
    kprint_cursor_pos = 0;
}

// Set kprint's internal cursor position
void kprint_set_position(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        kprint_cursor_pos = y * VGA_WIDTH + x;
        update_cursor_pos();
    }
}
