#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

////////////////////////////////////////
// I/O Port Access (x86 Architecture)
////////////////////////////////////////

// Send a byte to the specified I/O port
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}

// Read a byte from the specified I/O port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Send a 16-bit word to the specified I/O port
static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" :: "a"(val), "Nd"(port));
}

// Read a 16-bit word from the specified I/O port
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Perform a brief delay using port 0x80 (legacy method)
static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif // PORT_IO_H
