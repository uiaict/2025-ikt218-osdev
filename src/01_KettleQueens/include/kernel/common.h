#ifndef UIAOS_COMMON_H
#define UIAOS_COMMON_H

#include "libc/stdint.h"

// Read a byte from a port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Write a byte to a port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif // UIAOS_COMMON_H
