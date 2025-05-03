#ifndef UTIL_H
#define UTIL_H

#include "libc/stdint.h"

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif 
