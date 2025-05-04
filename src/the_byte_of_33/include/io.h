#ifndef IO_H
#define IO_H

#include <libc/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void set_color(uint8_t colour);
void puts(const char *s);
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif