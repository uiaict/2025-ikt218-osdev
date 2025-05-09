#ifndef PRINTF_H
#define PRINTF_H

#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void clear_screen();

// Viser tekst til skjermen
void printf(const char *message, ...);
void error_message(const char *message, ...);

void update_cursor(int x, int y);

// Disse må være tilgjengelig eksternt
extern volatile char *vga;
extern int cursor;

#endif // PRINTF_H
