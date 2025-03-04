#include "libc/stdint.h"      // Inkluderer standard integer-typer som uint8_t og uint16_t
#include "miscFuncs.h"    // Inkluderer headerfilen med funksjonene vi definerer her

// Adressen til VGA-tekstbufferen i minnet (fast for tekstmodus)
#define VGA_ADDRESS 0xB8000

// Standard VGA-tekstmodus har 80 kolonner og 25 rader
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Peker til VGA-minnet, hvor vi kan skrive tegn som vises på skjermen
static uint16_t* terminal_buffer = (uint16_t*)VGA_ADDRESS;

// Variabler for å holde styr på hvor vi skal skrive neste tegn
static uint8_t terminal_row = 0, terminal_column = 0;

// Standard farge for tekst (hvit på svart bakgrunn)
static uint8_t terminal_color = COLOR_WHITE;

/**
 * Initialiserer terminalen ved å fylle hele skjermen med blanke tegn (' ')
 * og sette standardfargen til hvit.
 */
void terminal_initialize() {
    // Gå gjennom hver rad og kolonne og fyll skjermen med mellomrom
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = (COLOR_WHITE << 8) | ' ';
        }
    }

    // Nullstill posisjon og farge
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = COLOR_WHITE;
}

/**
 * Skriver ett tegn til skjermen på gjeldende posisjon.
 * Hvis tegnet er '\n', går vi til neste linje.
 */
void terminal_write_char(char c) {
    if (c == '\n') { // Hvis vi får et linjeskift, flytt til neste linje
        terminal_row++;
        terminal_column = 0;
        return;
    }
    
    // Beregn posisjonen i VGA-minnet
    const int index = terminal_row * VGA_WIDTH + terminal_column;
    
    // Sett tegnet med gjeldende farge i VGA-minnet
    terminal_buffer[index] = (terminal_color << 8) | c;
    
    // Gå til neste posisjon
    terminal_column++;
    
    // Hvis vi har nådd slutten av linjen, gå til neste linje
    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    
    // Enkel scrolling hvis vi når bunnen av skjermen (ikke optimal implementasjon)
    if (terminal_row == VGA_HEIGHT) {
        terminal_row = VGA_HEIGHT - 1;
    }
}

/**
 * Skriver en hel streng til skjermen, tegn for tegn.
 */
void terminal_write(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        terminal_write_char(str[i]);
    }
}

/**
 * Skriver en streng med en spesifisert farge.
 * Lagrer den originale fargen, endrer til den ønskede, skriver strengen, og gjenoppretter fargen.
 */
void terminal_write_color(const char* str, TerminalColor color) {
    uint8_t originalColor = terminal_color;
    terminal_color = color;
    terminal_write(str);
    terminal_color = originalColor;
}

/**
 * Konverterer et tall til en heksadesimal streng.
 * Resultatet blir lagret i str-parameteren, som må ha plass til minst 11 tegn.
 */
void hexToString(uint32_t num, char* str) {
    const char* hex_chars = "0123456789ABCDEF";
    str[0] = '0';
    str[1] = 'x';
    
    for (int i = 0; i < 8; i++) {
        str[9-i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[10] = '\0';
}

/**
 * Enkel forsinkelse som brukes for å gi tid mellom operasjoner.
 * Dette er ikke en nøyaktig tidsforsinkelse, men gir en relativ pause.
 */
void delay(int count) {
    // Enkel busy-wait loop
    for (volatile int i = 0; i < count * 1000000; i++) {
        // Gjør ingenting, bare vent
    }
}