#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"   // Include standard integer types like uint16_t, uint32_t


// Funksjon for å skrive én karakter til skjermen
void terminal_putc(char c);

// Funksjon for å skrive en hel streng til skjermen
void terminal_write(const char* str);

#endif /* TERMINAL_H */
