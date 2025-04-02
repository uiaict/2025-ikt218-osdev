#ifndef putchar_h
#define putchar_h

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;

void putchar(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        return;
    }

    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[pos] = (uint16_t)c | (0x07 << 8); 

    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
    }
}
#endif