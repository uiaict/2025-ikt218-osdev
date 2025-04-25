#ifndef IO_H
#define IO_H

#include <libc/stdint.h>

void set_color(uint8_t colour);
void puts(const char *s);
void printf(const char* fmt, uint32_t arg);
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif