#include "libc/io.h"

// Write a byte to the specified port
void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}


// Read a byte from the specified port
uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}
