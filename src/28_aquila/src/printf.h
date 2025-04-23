#ifndef PRINTF_H
#define PRINTF_H

#include "libc/stdint.h"

// Viser tekst til skjermen
void printf(const char *message);

void update_cursor(int x, int y);

// Disse må være tilgjengelig eksternt
extern volatile char *vga;
extern int cursor;

#endif // PRINTF_H
