#include "drivers/terminal.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "sys/io.h"

int col = 0;
int row = 0;

// https://wiki.osdev.org/Printing_To_Screen
uint16_t *video = 0xB8000;

void terminal_clear() {
    row = 0;
    col = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            terminal_put(' ', WHITE, x, y);
        }
    }
}

void terminal_put(char c, int color, int x, int y) {
    uint16_t entry = (color << 8) | (uint16_t)c;
    video[y * WIDTH + x] = entry;
}

void terminal_write(int color, const char *str) {
    int i = 0;
    while (str[i]) {
        if (str[i] == '\n') {
            col = 0;     
            row++;

            if (row == HEIGHT + 1) {
                terminal_scroll_down();
            }

        } else {
            terminal_put(str[i], color, col, row);
            col++;
        }

        if (col == WIDTH) {
            col = 0;
            row++;

            if (row == HEIGHT + 1) {
                terminal_scroll_down();
            }
        }

        i++;
    }
}

void terminal_scroll_down() {
    for (int y = 1; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            video[((y - 1) * WIDTH + x) * 2] = video[((y) * WIDTH + x) * 2];
            video[((y - 1) * WIDTH + x) * 2 + 1] = video[((y) * WIDTH + x) * 2 + 1];
        }
    }

    row--;
}


void reverse(char *str, int len) {
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        end--;
        start++;
    }
}

// https://www.geeksforgeeks.org/implement-itoa/
void itoa(int num, char *str, int base) {
    int i = 0;
    bool is_neg = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10) {
        is_neg = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_neg) {
        str[i++] = '-';
    }

    str[i] = '\0';
    
    reverse(str, i);
}

// https://www.geeksforgeeks.org/convert-floating-point-number-string/?ref=ml_lbp
void ftoa(float num, char *str, int afterpoint) {
    int ipart = (int) num;
    float fpart = num - (float) ipart;

    itoa(ipart, str, 10);
    
    int i = strlen(str);

    if (afterpoint != 0) {
        str[i] = '.';
        i++;
        for (int j = 0; j < afterpoint; j++) {
            fpart = fpart * 10;
            int frac = (int)fpart;
            str[i++] = frac + '0';
            fpart -= frac;
        }
    }

    str[i] = '\0';
}




// https://wiki.osdev.org/Text_Mode_Cursor

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void update_cursor(int x, int y) {
    uint16_t pos = y * WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
