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
  if (cursorPositionY_ >= SCREEN_HEIGHT) {
    cursorPositionY_ = 0;
  }
}

void print(const char *string) {
  volatile char *video = (volatile char *)0xB8000;
  int str_i = 0;

  while (string[str_i] != 0) {
    if (string[str_i] == '\n') {
      cursorPositionY_++;
      if (cursorPositionY_ >= SCREEN_HEIGHT)
	cursorPositionY_ = 0;
      cursorPositionX_ = 0;
      clear_line(cursorPositionY_);
      str_i++;
      continue;
    }
    video = cursorPosToAddress_(cursorPositionX_, cursorPositionY_);
    *video = string[str_i];
    video++;
    *video = VIDEO_WHITE;

    str_i++;
    incrementCursorPosition_();
  }
}

void clear_line(int line) {
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    volatile char *video = cursorPosToAddress_(x, line);
    *video = ' ';
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
