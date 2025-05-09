#include "libc/util.h"

// Set a block of memory to a specific value
void *memset(void *dest, unsigned char val, uint32_t count)  // Change 'char' to 'unsigned char' to match the declaration
{
    unsigned char *temp = (unsigned char*) dest;  // Use 'unsigned char' for temp pointer
    for (; count != 0; count--) 
    {
        *temp++ = val;
    }
    return dest;  // Return the destination pointer, which is the typical behavior for memset
}

// Write a byte to an I/O port
void outPortB(uint16_t port, uint8_t value) 
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

char inPortB(uint16_t port) 
{
    char rv;
    asm volatile("inb %1, %0" : "=a"(rv) : "dN"(port));
    return rv;
}
