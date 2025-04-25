#include "print.h"

int cursorPositionX_ = 0;
int cursorPositionY_ = 0;

char *cursorPosToAddress(int x, int y) {
  return (char *)(0xB8000 + ((x + y * SCREEN_WIDTH) * 2));
}

void incrementCursorPosition() {
  cursorPositionX_++;
  if (cursorPositionX_ >= SCREEN_WIDTH) {
    cursorPositionY_++;
    cursorPositionX_ = 0;
  }
  if (cursorPositionY_ >= SCREEN_HEIGHT) {
    cursorPositionY_ = 0;
  }
}

void print(const char *string) { printc(string, VIDEO_WHITE); }

void printc(const char *string, VideoColour colour) {
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
    video = cursorPosToAddress(cursorPositionX_, cursorPositionY_);
    *video = string[str_i];
    video++;
    *video = colour;

    str_i++;
    incrementCursorPosition();
  }
}

void clear_line(int line) {
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    volatile char *video = cursorPosToAddress(x, line);
    *video = ' ';
  }
}