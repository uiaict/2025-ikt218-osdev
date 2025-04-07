

// io.h
#pragma once
#include "libc/stdint.h" // For uint8_t og uint16_t

uint8_t inPortB(uint16_t port);
void outb(uint16_t port, uint8_t data);
