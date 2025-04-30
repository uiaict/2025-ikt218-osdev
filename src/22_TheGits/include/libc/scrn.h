#ifndef SCRN_H
#define SCRN_H

#include "libc/stdint.h"
#include "pit/pit.h"
#include "libc/scrn.h"
#include "libc/io.h"
#include "libc/isr_handlers.h"
#include "libc/stdarg.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Fargekoder for tekst og bakgrunn
#define VGA_COLOR(fg, bg) ((bg << 4) | (fg))
#define VGA_ENTRY(c, color) ((uint16_t)c | (uint16_t)color << 8)
// VGA-minneadresse
#define VGA_MEMORY (uint16_t*)0xB8000
#define VGA_WIDTH 80
#define INPUT_BUFFER_SIZE 128


void scrn_init_input_buffer();
void scrn_store_keypress(char c);
void get_input(char* buffer, int max_len);

void scrn_set_shift_pressed(bool value);
bool scrn_get_shift_pressed();


void terminal_write(const char* str, uint8_t color);
void printf(const char* format, ...);

void panic(const char* message);

int strcmp(const char* str1, const char* str2);
char *strcpy(char* dest, const char* src);

#endif // SCRN_H