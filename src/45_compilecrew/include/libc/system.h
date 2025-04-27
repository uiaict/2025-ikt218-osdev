#ifndef SYSTEM_H
#define SYSTEM_H

#include "libc/stdint.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

#endif // SYSTEM_H
