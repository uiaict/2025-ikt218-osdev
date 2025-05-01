#ifndef KPRINT_H
#define KPRINT_H

void kprint(const char *str);
void kprint_hex(unsigned long num);
void kprint_dec(unsigned long num);
void kprint_clear(void);
void kprint_set_position(int x, int y);

#endif
