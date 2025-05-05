#ifndef UTILS_H
#define UTILS_H

#include <libc/stdint.h>
#include "pit.h"

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline void io_wait(void) {
    outb(0x80, 0);
}

extern int rand(void);
extern int srand(unsigned int seed);

#endif /* UTILS_H */