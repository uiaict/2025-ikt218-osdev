#ifndef COMMON_H
#define COMMON_H

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

void memset(u8int *dest, u8int val, u32int len);
void memcpy(u8int *dest, const u8int *src, u32int len);

void panic(const char *message);
void enable_interrupts();

#endif
