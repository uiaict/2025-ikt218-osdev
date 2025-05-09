#ifndef KERNEL_COMMON_H
#define KERNEL_COMMON_H

#include <libc/stdint.h>

////////////////////////////////////////
// I/O Port Operations
////////////////////////////////////////

// Write a byte to the specified I/O port
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read a byte from the specified I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // KERNEL_COMMON_H
