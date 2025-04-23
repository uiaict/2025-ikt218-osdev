#include "display.h"
#include "libc/string.h"
#include "interruptHandler.h"

// VGA text mode buffer address
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// VGA dimensions
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Terminal variables
size_t terminal_row;
size_t terminal_column;
static uint8_t terminal_color;

// Create a VGA entry with the given character and color
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Create a VGA color byte from foreground and background colors
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

void display_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    
    // Clear screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
    
    display_move_cursor();
}

void display_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    display_move_cursor();
}

void display_set_color(vga_color_t fg, vga_color_t bg) {
    terminal_color = vga_color(fg, bg);
}

void display_putchar(char c) {
    display_write_char(c);
}

void display_write_char(char c) {
    // Handle special characters
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            // Scroll screen
            for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    VGA_MEMORY[y * VGA_WIDTH + x] = VGA_MEMORY[(y + 1) * VGA_WIDTH + x];
                }
            }
            // Clear last line
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
            }
            terminal_row = VGA_HEIGHT - 1;
        }
    }
    else if (c == '\r') {
        terminal_column = 0;
    }
    else if (c == '\t') {
        // Tab = 4 spaces
        for (int i = 0; i < 4; i++) {
            display_write_char(' ');
        }
    }
    else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);
        }
        else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);
        }
    }
    else {
        VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                // Scroll screen
                for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        VGA_MEMORY[y * VGA_WIDTH + x] = VGA_MEMORY[(y + 1) * VGA_WIDTH + x];
                    }
                }
                // Clear last line
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
                }
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
    display_move_cursor();
}

void display_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        display_write_char(data[i]);
    }
}

void display_writestring(const char* data) {
    display_write(data);
}

void display_write_string(const char* str) {
    display_write(str);
}

void display_write_color(const char* str, vga_color_t color) {
    terminal_color = vga_color(color, COLOR_BLACK);
    display_write(str);
}

void display_write_decimal(int num) {
    if (num == 0) {
        display_write_char('0');
        return;
    }
    
    if (num < 0) {
        display_write_char('-');
        num = -num;
    }
    
    char buffer[32];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (--i >= 0) {
        display_write_char(buffer[i]);
    }
}

void display_write_hex(uint32_t num) {
    display_write("0x");
    
    if (num == 0) {
        display_write_char('0');
        return;
    }
    
    char buffer[8];
    int i = 0;
    
    while (num > 0) {
        int digit = num & 0xF;
        buffer[i++] = digit < 10 ? '0' + digit : 'A' + (digit - 10);
        num >>= 4;
    }
    
    while (--i >= 0) {
        display_write_char(buffer[i]);
    }
}

void display_move_cursor(void) {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    
    // Send low byte
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    
    // Send high byte
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void display_write_char_color(char c, vga_color_t color) {
    uint8_t old_color = terminal_color;
    terminal_color = vga_color(color, COLOR_BLACK);
    display_write_char(c);
    terminal_color = old_color;
}

// Display the boot logo
void display_boot_logo(void) {
    display_write_color("\n\n\n\n", COLOR_WHITE);
    
    // ASCII art logo for "Sweater OS" provided by user - perfectly centered for the screen
    display_write_color("                 _____                   _               ____   _____\n", COLOR_CYAN);
    display_write_color("                / ____|                 | |             / __ \\ / ____|\n", COLOR_CYAN);
    display_write_color("               | (_____      _____  __ _| |_ ___ _ __  | |  | | (___  \n", COLOR_CYAN);
    display_write_color("                \\___ \\ \\ /\\ / / _ \\/ _` | __/ _ \\ '__| | |  | |\\___ \\ \n", COLOR_WHITE);
    display_write_color("                ____) \\ V  V /  __/ (_| | ||  __/ |    | |__| |____) |\n", COLOR_CYAN);
    display_write_color("               |_____/ \\_/\\_/ \\___|\\__,_|\\__\\___|_|     \\____/|_____/ \n", COLOR_CYAN);
    display_write_color("\n\n\n\n", COLOR_WHITE);
    
    // Tagline - centered to match the logo
    display_write_color("                              A COZY EXPERIENCE                        \n", COLOR_LIGHT_GREEN);
    display_write_color("\n\n\n", COLOR_WHITE);
} 