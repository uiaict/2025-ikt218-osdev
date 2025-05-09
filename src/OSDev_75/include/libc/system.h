#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stddef.h>

// Basic port I/O functions
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Print functions (assuming vga.h or similar provides these)
void print(const char* str);
void printf(const char* format, ...);

// Panic function
void panic(const char* message);

#endif // SYSTEM_H