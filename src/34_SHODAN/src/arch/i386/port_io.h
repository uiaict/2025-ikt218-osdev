// === port_io.h ===
#ifndef PORT_IO_H
#define PORT_IO_H

#include <stdint.h>

// Write a byte to the specified port
void outb(uint16_t port, uint8_t value);

// Read a byte from the specified port
uint8_t inb(uint16_t port);

#endif

