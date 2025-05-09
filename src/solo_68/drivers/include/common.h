#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
int strcmp(const char* s1, const char* s2);
int strlen(const char* str);
void int_to_string(int value, char* buffer);

#endif
