#include "system.h"
#include "print.h"

void *malloc(int bytes) { return 0; }

void free(void *pointer) {}

void cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void cursor_disable() {
  /*
asm("mov $0x01, %AH;"
    "mov $0x3F, %CH;"
    "int $0x10;");
    */
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void update_cursor(int x, int y) {
  uint16_t pos = y * SCREEN_WIDTH + x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void) {
  uint16_t pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((uint16_t)inb(0x3D5)) << 8;
  return pos;
}
