#ifndef SYSTEM_H
#define SYSTEM_H
#include "libc/stdint.h"


void shutdown();

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}


#endif