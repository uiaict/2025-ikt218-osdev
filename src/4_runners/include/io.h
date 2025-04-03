#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

void outb(uint16_t port, uint8_t value);

#endif // IO_H