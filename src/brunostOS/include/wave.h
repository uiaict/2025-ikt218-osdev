#ifndef WAVE_H
#define WAVE_H

#include "libc/stdint.h"

#define PIXEL_COUNT (VGA_HEIGHT-2)*VGA_WIDTH*2 // Not actually pixels
#define STORAGE_SPACE PIXEL_COUNT + sizeof(struct save_header) + 500 // Required space for storage

struct save_header{
    uint16_t magic;
    unsigned char filename[21];
};

static const uint16_t magic = 0b1010101010101010;

void paint(char[], char[]);
void print_menu();

void savemenu_clear(int);
void save_painting(char[], char[]);
void load_painting(char[], char[]);

#endif // WAVE_H