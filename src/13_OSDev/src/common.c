#include "common.h"
#include "libc/system.h"

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5
#define VGA_OFFSET_HIGH  14
#define VGA_OFFSET_LOW   15


// Assignment 6
void move_cursor(uint32_t x, uint32_t y) {
    uint16_t position = y * 80 + x;

    outb(VGA_COMMAND_PORT, VGA_OFFSET_HIGH);
    outb(VGA_DATA_PORT, (uint8_t)(position >> 8));

    outb(VGA_COMMAND_PORT, VGA_OFFSET_LOW);
    outb(VGA_DATA_PORT, (uint8_t)(position & 0xFF));
}


void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}