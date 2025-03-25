#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

// Function to write to an I/O port (inline assembly)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif // IO_H