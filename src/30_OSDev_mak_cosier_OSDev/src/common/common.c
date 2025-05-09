#include "../../include/libc/common.h"

// Write a byte to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Read a byte from the specified port.
u8int inb(u16int port)
{
    u8int ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Read a word (16 bits) from the specified port.
u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy n bytes from source to destination.
void *memcpy(void *dest, const void *src, u32int n)
{
    u8int *d = (u8int *)dest;
    const u8int *s = (const u8int *)src;
    while (n--)
    {
        *d++ = *s++;
    }
    return dest;
}

// Set n bytes of the memory area pointed to by dest to the specified value.
/*void *memset(void *dest, s32int val, u32int n)
{
    u8int *d = (u8int *)dest;
    while (n--)
    {
        *d++ = (u8int)val;
    }
    return dest;
}*/
