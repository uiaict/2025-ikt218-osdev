#include "libc/terminal.h"
#include "libc/system.h"
#include "libc/stdarg.h"
#include "libc/stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* vga_buffer = (uint16_t*) VGA_ADDRESS;
static uint16_t terminal_row = 0;
static uint16_t terminal_column = 0;
static uint8_t terminal_color = 0x07; // Light grey on black

// Write a character and color into VGA memory
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | ((uint16_t) color << 8);
}

// Move hardware cursor
static void move_cursor() {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_backspace(void) {
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    } else {
        return; // Already at top-left corner, can't move back
    }

    // Overwrite current character with a space
    vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);

    // Update cursor
    move_cursor();
}

// Scroll screen up by one line
static void scroll() {
    uint8_t attributeByte = (0 << 4) | (7 & 0x0F); // Black bg, light grey text
    uint16_t blank = 0x20 | (attributeByte << 8); // Space character

    if (terminal_row >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }

        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = blank;
        }

        terminal_row = VGA_HEIGHT - 1;
    }
}

// Initialize terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = 0x07; // Light grey on black
    vga_buffer = (uint16_t*) VGA_ADDRESS;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }

    move_cursor();
}

// Put a single character on screen
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    scroll();
    move_cursor();
}

// Write a string to the terminal
void terminal_write(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = ((uint16_t)color << 8) | (uint8_t)c;
}

void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat(' ', terminal_color, x, y);
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

void draw_front_page() {
    terminal_clear();
    
    
    // Centered title
    const char* title = "Welcome to JooaOS";
    const size_t title_len = strlen(title);
    size_t x = (80 - title_len) / 2;
    size_t y = 5;
    for (size_t i = 0; i < title_len; i++) {
        terminal_putentryat(title[i], 0x0F, x + i, y); // white on black
    }

    // Subtitle
    const char* subtitle = "IKT218 - Operating Systems Project";
    x = (80 - strlen(subtitle)) / 2;
    y += 2;
    for (size_t i = 0; i < strlen(subtitle); i++) {
        terminal_putentryat(subtitle[i], 0x07, x + i, y); // light grey
    }

    // Prompt (multi-line)
    const char* lines[] = {
        "[1] Matrix Rain",
        "[2] Music",
        "[3] Memory layout",
        "[4] Empty terminal",
        "[Q] Quit"
    };

    for (int i = 0; i < 5; i++) {
        const char* line = lines[i];
        x = (80 - strlen(line)) / 2;
        y += 2;
        for (size_t j = 0; j < strlen(line); j++) {
            terminal_putentryat(line[j], 0x08, x + j, y); // dark grey
        }
    }
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t start, uint8_t end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

void draw_music_selection(){
    terminal_clear();
    printf("[1] song 1\n[2] song 2\n[3] song 3\n[esc] Back to main menu");
}