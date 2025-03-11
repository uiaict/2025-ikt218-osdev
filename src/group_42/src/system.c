#include "system.h"

void *malloc(int bytes) { return 0; }

void free(void *pointer) {}

static int cursorPositionX_ = 0;
static int cursorPositionY_ = 0;

char *cursorPosToAddress_(int x, int y) {
  return (char *)(0xB8000 + ((x + y * SCREEN_WIDTH) * 2));
}

void incrementCursorPosition_() {
  cursorPositionX_++;
  if (cursorPositionX_ >= SCREEN_WIDTH) {
    cursorPositionY_++;
    cursorPositionX_ = 0;
  }
}

void print(const char *string) {
  volatile char *video = (volatile char *)0xB8000;

  while (string != 0) {
    if (*string == '\n') {
      cursorPositionY_++;
      cursorPositionX_ = 0;
      string++;
      continue;
    }
    video = cursorPosToAddress_(cursorPositionX_, cursorPositionY_);
    *video = *string;
    video++;
    *video = VIDEO_WHITE;

    string++;
    incrementCursorPosition_();
  }
}

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
