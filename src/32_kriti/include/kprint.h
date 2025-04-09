#ifndef KPRINT_H
#define KPRINT_H

#include <libc/stdint.h>

void kprint(const char *str);
void kprint_hex(unsigned long num);  // Changed to unsigned long instead of uint64_t
void kprint_dec(unsigned long num);  // Changed to unsigned long instead of uint64_t
void kprint_clear(void);
void kprint_set_position(int x, int y);

#endif /* KPRINT_H */