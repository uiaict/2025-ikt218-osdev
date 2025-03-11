#ifndef MISC_FUNCS_H
#define MISC_FUNCS_H

#include "libc/stdint.h" // Standard integer-typer for portabilitet

// Define terminal colors
typedef enum {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GRAY = 7,
    COLOR_GRAY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
    
    // Ekstra fargekombinasjoner for spesielle tilfeller
    COLOR_YELLOW = 0x0E,  // Gul tekst på svart bakgrunn
    COLOR_BLACK_ON_WHITE = 0xF0, // Svart tekst på hvit bakgrunn
    COLOR_BLACK_ON_GREEN = 0x20, // Svart tekst på grønn bakgrunn
    COLOR_BLACK_ON_BLUE = 0x10,  // Svart tekst på blå bakgrunn
    COLOR_BLACK_ON_RED = 0x40    // Svart tekst på rød bakgrunn
} VGA_COLOR;

// Rydder skjermen og setter standard tekstfarge til hvit
void terminal_initialize();

// Skriver ut en tekststreng med standardfarge
void terminal_write(const char* str);

// Skriver ut ett enkelt tegn med standardfarge
void terminal_write_char(char c);

// Skriver ut en tekststreng med en spesifisert farge
void terminal_write_color(const char* str, VGA_COLOR color);

// Konverterer et heltall til en heksadesimal streng
void hexToString(uint32_t num, char* str);

// Skriver ut et tall i heksadesimal format
void terminal_write_hex(uint32_t num);

// Skriver ut et tall i desimal format
void terminal_write_decimal(uint32_t num);

// Skriver ut en tekststreng
void terminal_write_string(const char* str);

// Scrollfunksjon for terminalen
void terminal_scroll();

// Forsinkelse (venter i omtrent ms millisekunder)
void delay(uint32_t ms);

// Stopper CPU'en i tilfelle kritisk feil
void halt();

// Verifiserer at operativsystemet ble startet av en kompatibel bootloader
void verify_boot_magic(uint32_t magic);

// Initialiserer systemets grunnleggende komponenter som GDT og IDT
void initialize_system();

/**
 * Forsøker å initialisere systemet med feilhåndtering
 * 
 * Denne funksjonen initialiserer de ulike delene av systemet i riktig rekkefølge
 * og med feilhåndtering for å sikre at systemet starter opp på en robust måte.
 */
void try_initialize_system();

#endif /* MISC_FUNCS_H */
