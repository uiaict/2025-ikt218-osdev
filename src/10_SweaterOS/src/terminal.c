#include <stdint.h>      // Inkluderer standard integer-typer som uint8_t og uint16_t
#include "terminal.h"    // Inkluderer headerfilen med funksjonene vi definerer her

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
    } else {
        // Skriver tegnet til riktig posisjon i VGA-minnet, med fargen vi har satt
        terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] = (terminal_color << 8) | c;
        terminal_column++; // Flytt markøren til neste kolonne
    }

    // Hvis vi når slutten av en linje, gå til neste rad
    if (terminal_column >= VGA_WIDTH) {
        terminal_row++;
        terminal_column = 0;
    }

    // Hvis vi når bunnen av skjermen, nullstill skjermen (kan byttes ut med scrolling senere)
    if (terminal_row >= VGA_HEIGHT) {
        terminal_initialize();
    }
}

/**
 * Skriver en hel streng til skjermen ved å kalle `terminal_write_char()`
 * for hvert tegn i strengen.
 */
void terminal_write(const char* str) {
    while (*str) { // Gå gjennom hele strengen og skriv ut ett og ett tegn
        terminal_write_char(*str++);
    }
}

/**
 * Skriver en farget streng til skjermen.
 * Bruker en fargeverdi (enum TerminalColor) til å sette tekstfargen.
 */
void terminal_write_color(const char* str, TerminalColor color) {
    terminal_color = color; // Endre til ønsket farge
    terminal_write(str); // Skriv ut strengen
    terminal_color = COLOR_WHITE; // Tilbakestill til standard hvit farge etterpå
}