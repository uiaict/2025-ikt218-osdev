#include "../include/libc/util.h"

// Set a block of memory to a specific value
void memset(void *dest, char val, uint32_t count) 
{
    char *temp = (char*) dest;
    for (; count != 0; count--) 
    {
        *temp++ = val;
    }
}

// Write a byte to an I/O port
void outPortB(uint16_t port, uint8_t value) 
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}