#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

uint8_t inb(uint16_t port);         // ✅ REQUIRED
void outb(uint16_t port, uint8_t value);

#endif // IO_H
