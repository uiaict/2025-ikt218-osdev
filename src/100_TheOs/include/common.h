#ifndef COMMON_H
#define COMMON_H

#include "libc/stdint.h"

// This header file defines functions for common.c

// Send a byte to I/O port
void outb(uint16_t port, uint8_t value);
// Read a byte from I/O port
uint8_t inb(uint16_t port);
// Read a word (2 bytes) to I/O port
uint16_t inw(uint16_t port);

#endif
