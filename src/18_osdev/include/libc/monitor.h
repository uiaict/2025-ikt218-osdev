#pragma once

#include "stdint.h" 
#include "stddef.h" 

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void monitor_clear(void);
void monitor_put(char c,int color);
void monitor_remove_char();
void monitor_write(const char* str);
void monitor_write_color(int color, const char* str);
void monitor_write_dec(uint32_t n);
void monitor_write_hex(uint32_t num);