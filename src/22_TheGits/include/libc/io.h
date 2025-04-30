#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

// Write a byte to a port
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from a port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Wait a bit (often used after port operations)
static inline void io_wait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif // IO_H