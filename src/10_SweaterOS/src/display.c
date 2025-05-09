#include "display.h"
#include "libc/string.h"
#include "interruptHandler.h"
#include "programmableIntervalTimer.h"

// VGA tekstmodus buffer adresse
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// VGA skjermdimensjoner
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Terminal variabler
size_t terminal_row;
size_t terminal_column;
static uint8_t terminal_color;

// Oppretter en VGA oppføring med tegn og farge
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Oppretter en VGA fargebyte fra forgrunns- og bakgrunnsfarger
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

// Initialiserer skjermen
void display_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    
    // Tømmer skjermen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
    
    display_move_cursor();
}

// Tømmer skjermen
void display_clear(void) {
    // Fyller skjermen med mellomrom
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

// Setter terminalfargen
void display_set_color(vga_color_t fg, vga_color_t bg) {
    terminal_color = vga_color(fg, bg);
}

// Skriver et tegn til skjermen
void display_write_char(char c) {
    // Håndterer spesialtegn
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            // Ruller skjermen opp
            for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    VGA_MEMORY[y * VGA_WIDTH + x] = VGA_MEMORY[(y + 1) * VGA_WIDTH + x];
                }
            }
            // Tømmer nederste linje
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
        // Skriver 4 mellomrom for tab
        for (int i = 0; i < 4; i++) {
            display_write_char(' ');
        }
    }
    else if (c == '\b') {
        // Håndterer backspace
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
        // Skriver vanlig tegn
        VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                // Ruller skjermen opp
                for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
                    for (size_t x = 0; x < VGA_WIDTH; x++) {
                        VGA_MEMORY[y * VGA_WIDTH + x] = VGA_MEMORY[(y + 1) * VGA_WIDTH + x];
                    }
                }
                // Tømmer nederste linje
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                    VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
                }
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
}

// Skriver en streng til skjermen
void display_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        display_write_char(data[i]);
    }
    display_move_cursor();
}

// Funksjonsaliaser for bakoverkompatibilitet
void display_writestring(const char* data) { display_write(data); }
void display_write_string(const char* str) { display_write(str); }
void display_putchar(char c) { display_write_char(c); display_move_cursor(); }

// Skriver en streng med spesifisert farge
void display_write_color(const char* str, vga_color_t color) {
    uint8_t old_color = terminal_color;
    terminal_color = vga_color(color, COLOR_BLACK);
    display_write(str);
    terminal_color = old_color;
}

// Skriver et tegn med spesifisert farge
void display_write_char_color(char c, vga_color_t color) {
    uint8_t old_color = terminal_color;
    terminal_color = vga_color(color, COLOR_BLACK);
    display_write_char(c);
    terminal_color = old_color;
}

// Skriver et desimaltall til skjermen
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
    
    // Konverterer tall til siffer
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Skriver sifrene i riktig rekkefølge
    while (--i >= 0) {
        display_write_char(buffer[i]);
    }
}

// Skriver et heksadesimalt tall til skjermen
void display_write_hex(uint32_t num) {
    display_write("0x");
    
    if (num == 0) {
        display_write_char('0');
        return;
    }
    
    char buffer[8];
    int i = 0;
    
    // Konverterer tall til heksadesimale siffer
    while (num > 0) {
        int digit = num & 0xF;
        buffer[i++] = digit < 10 ? '0' + digit : 'A' + (digit - 10);
        num >>= 4;
    }
    
    // Skriver sifrene i riktig rekkefølge
    while (--i >= 0) {
        display_write_char(buffer[i]);
    }
}

// Oppdaterer markørposisjonen
void display_move_cursor(void) {
    static uint16_t last_pos = 0xFFFF;
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    
    if (pos != last_pos) {
        // Oppdaterer markørposisjon via VGA port
        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint8_t)(pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
        last_pos = pos;
    }
}

// Viser oppstartslogoen med animasjon
void display_boot_logo(void) {
    display_clear();
    display_write_color("\n\n\n\n\n\n\n\n\n\n           Loading SweaterOS...", COLOR_LIGHT_CYAN);
    sleep_interrupt(300);
    display_clear();
    display_write_color("\n\n\n\n", COLOR_WHITE);
    sleep_interrupt(150);
    display_write_color("                 _____                   _               ____   _____\n", COLOR_CYAN);
    sleep_interrupt(150);
    display_write_color("                / ____|                 | |             / __ \\ / ____|\n", COLOR_CYAN);
    sleep_interrupt(150);
    display_write_color("               | (_____      _____  __ _| |_ ___ _ __  | |  | | (___  \n", COLOR_CYAN);
    sleep_interrupt(150);
    display_write_color("                \\___ \\ \\ /\\ / / _ \\ / _` | __/ _ \\ '__| | |  | |\\___ \\ \n", COLOR_WHITE);
    sleep_interrupt(150);
    display_write_color("                ____) \\ V  V /  __/ (_| | ||  __/ |    | |__| |____) |\n", COLOR_CYAN);
    sleep_interrupt(150);
    display_write_color("               |_____/ \\_/\\_/ \\___|\\__,_|\\__\\___|_|     \\____/|_____/ \n", COLOR_CYAN);
    sleep_interrupt(300);
    display_write_color("\n\n\n                          A COZY EXPERIENCE                           \n", COLOR_LIGHT_GREEN);
    sleep_interrupt(400);
}

// Setter markørposisjon
void display_set_cursor(size_t x, size_t y) {
    // Sikrer at posisjonen er innenfor grensene
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    
    terminal_column = x;
    terminal_row = y;
    display_move_cursor();
}

// Hides the cursor
void display_hide_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
} 