#ifndef MISC_FUNCS_H
#define MISC_FUNCS_H

#include <stdint.h> // Standard integer-typer for portabilitet

// Definer farger som kan brukes for tekst på skjermen
typedef enum {
    COLOR_WHITE  = 0x0F,  // Hvit tekst på svart bakgrunn
    COLOR_RED    = 0x04,  // Rød tekst på svart bakgrunn
    COLOR_GREEN  = 0x02,  // Grønn tekst på svart bakgrunn
    COLOR_BLUE   = 0x01,  // Blå tekst på svart bakgrunn
    COLOR_YELLOW = 0x0E   // Gul tekst på svart bakgrunn
} TerminalColor;

// Rydder skjermen og setter standard tekstfarge til hvit
void terminal_initialize();

// Skriver ut en tekststreng med standardfarge
void terminal_write(const char* str);

// Skriver ut ett enkelt tegn med standardfarge
void terminal_write_char(char c);

// Skriver ut en tekststreng med en spesifisert farge
void terminal_write_color(const char* str, TerminalColor color);

#endif // MISC_FUNCS_H
