// monitor.h -- Basic screen output functions
#ifndef MONITOR_H
#define MONITOR_H

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_ADDR 0xB8000
#define WHITE_ON_BLACK 0x0F



#include "libc/stdint.h"
void monitor_put(char c);
void monitor_write(const char *string);
void monitor_newline();
void monitor_backspace();
void monitor_write_hex(uint32_t n);
void monitor_write_dec(uint32_t n);
void monitor_put_for_matrix(char c, int x, int y, uint8_t color);

#endif