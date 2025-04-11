#ifndef COMMON_H
#define COMMON_H

// Standard typedefs for 32-bit x86 systems.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

// I/O port functions.
void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

// Memory functions.
void *memcpy(void *dest, const void *src, u32int n);
void *memset(void *dest, unsigned char val, u32int n); // Change to unsigned char to match common practice

#endif // COMMON_H
