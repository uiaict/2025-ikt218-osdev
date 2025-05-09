#include "common.h"

// writes a byte (uint8_t) to a specified I/O port (uint16_t).
void outb(uint16_t port, uint8_t value)
{
   asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

// writes a byte (uint16_t) to a specified I/O port (uint16_t).
void outw(uint16_t port, uint16_t value)
{
   asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

// reads a byte (uint8_t) from a specified I/O port (uint16_t).
uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
   return ret;
}

//  reads a word (uint16_t) from a specified I/O port
uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
   return ret;
}