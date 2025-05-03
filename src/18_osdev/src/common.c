
// #include "../include/libc/common.h"

// void outb(uint16_t port, uint8_t value)
// {
//     asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
// }

// uint8_t inb(uint16_t port)
// {
//    uint8_t ret;
//    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
//    return ret;
// }

// uint16_t inw(uint16_t port)
// {
//    uint16_t ret;
//    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
//    return ret;
// }





// followd https://wiki.osdev.org/Inline_Assembly/Examples#I/O_access for I/O access functions
#include "../include/libc/common.h"
#include "libc/stdbool.h"

bool piano_mode_enabled = false;

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}


uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}