#pragma once 

#include "libc/stdint.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void soutw(uint16_t port, uint16_t value);