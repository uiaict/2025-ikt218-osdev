#ifndef MISC_FUNCS_H
#define MISC_FUNCS_H

#include "libc/stdint.h" // Standard integer-typer for portabilitet

// Definer farger som kan brukes for tekst på skjermen
typedef enum {
    COLOR_WHITE  = 0x0F,  // Hvit tekst på svart bakgrunn
    COLOR_RED    = 0x04,  // Rød tekst på svart bakgrunn
    COLOR_GREEN  = 0x02,  // Grønn tekst på svart bakgrunn
    COLOR_BLUE   = 0x01,  // Blå tekst på svart bakgrunn
    COLOR_YELLOW = 0x0E,  // Gul tekst på svart bakgrunn
    COLOR_CYAN   = 0x03,  // Cyan tekst på svart bakgrunn
    COLOR_MAGENTA = 0x05, // Magenta tekst på svart bakgrunn
    COLOR_GRAY   = 0x08,  // Grå tekst på svart bakgrunn
    COLOR_LIGHT_GREEN = 0x0A, // Lysegrønn tekst på svart bakgrunn
    COLOR_LIGHT_BLUE = 0x09,  // Lyseblå tekst på svart bakgrunn
    COLOR_LIGHT_CYAN = 0x0B,  // Lysecyan tekst på svart bakgrunn
    COLOR_BLACK_ON_WHITE = 0xF0, // Svart tekst på hvit bakgrunn
    COLOR_BLACK_ON_GREEN = 0x20, // Svart tekst på grønn bakgrunn
    COLOR_BLACK_ON_BLUE = 0x10,  // Svart tekst på blå bakgrunn
    COLOR_BLACK_ON_RED = 0x40    // Svart tekst på rød bakgrunn
} TerminalColor;

// Rydder skjermen og setter standard tekstfarge til hvit
void terminal_initialize();

// Skriver ut en tekststreng med standardfarge
void terminal_write(const char* str);

// Skriver ut ett enkelt tegn med standardfarge
void terminal_write_char(char c);

// Skriver ut en tekststreng med en spesifisert farge
void terminal_write_color(const char* str, TerminalColor color);

// Konverterer et heltall til en heksadesimal streng
void hexToString(uint32_t num, char* str);

// Enkel forsinkelse/delay-funksjon
void delay(int count);

#endif // MISC_FUNCS_H
