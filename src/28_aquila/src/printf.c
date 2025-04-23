#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char *vga = (volatile char *)VGA_ADDRESS;
int cursor = 0;

extern void outb(uint16_t port, uint8_t value);

void update_cursor(int x, int y) {
  uint16_t pos = y * VGA_WIDTH + x;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void printf(const char *message) {
  for (int i = 0; message[i] != '\0'; i++) {

    if (message[i] == '\n') {
        cursor = (cursor / VGA_WIDTH + 1) * VGA_WIDTH; // current_line = cursor / vga_width
    }
    else {
        vga[cursor * 2] = message[i];
        vga[cursor * 2 + 1] = 0x07; // Set color
        cursor++;
    }

    // Simple scrolling logic, temp?
    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
        cursor = 0; 
    }

    // Update cursor position
    int x = cursor % VGA_WIDTH;
    int y = cursor / VGA_WIDTH;
    update_cursor(x, y);
  }
}
