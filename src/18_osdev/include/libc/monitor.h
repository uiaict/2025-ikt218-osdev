#pragma once
#include "stdint.h" // <--- includes uint32_t
#include "stddef.h" // <--- includes size_t

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;


void monitor_initialize(void);
void monitor_clear(void);
void monitor_put(char c);
void monitor_write(const char* str);
void monitor_write_dec(uint32_t n);
