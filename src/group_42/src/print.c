#include "kernel/print.h"
#include "libc/stdarg.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"


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

void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  const char *p = format;
  char char_buf[2];
  char num_buf[12];
  char hex_buf[9];

  while (*p != '\0') {
    if (*p == '%') {
      p++;
      switch (*p) {
        case 'c': {
          char val = (char)va_arg(args, int);
          char_buf[0] = val;
          char_buf[1] = '\0';
          printc(char_buf, VIDEO_WHITE);
          break;
        }
        case 's': {
          char *str = va_arg(args, char *);
          printc(str, VIDEO_WHITE);
          break;
        }
        case 'd': {
          int num = va_arg(args, int);
          int i = 0;
          int is_negative = false;

          if (num < 0) {
            is_negative = true;
            num = -num;
          }

          do {
            num_buf[i++] = (num % 10) + '0';
            num /= 10;
          } while (num != 0);

          if (is_negative) {
            num_buf[i++] = '-';
          }

          num_buf[i] = '\0';

          for (int j = 0; j < i / 2; j++) {
            char temp = num_buf[j];
            num_buf[j] = num_buf[i - j - 1];
            num_buf[i - j - 1] = temp;
          }

          printc(num_buf, VIDEO_WHITE);
          break;
        }
        case 'x': {
          unsigned int num = va_arg(args, unsigned int);
          int i = 0;

          do {
            unsigned int nibble = num & 0xF;
            if (nibble < 10) {
              hex_buf[i++] = nibble + '0';
            } else {
              hex_buf[i++] = nibble - 10 + 'a';
            }
            num >>= 4;
          } while (num != 0);

          hex_buf[i] = '\0';

          for (int j = 0; j < i / 2; j++) {
            char temp = hex_buf[j];
            hex_buf[j] = hex_buf[i - j - 1];
            hex_buf[i - j - 1] = temp;
          }

          printc(hex_buf, VIDEO_WHITE);
          break;
        }
        case '%': {
           printc("%", VIDEO_WHITE);
          break;
        }
        default: {
          char_buf[0] = '%';
          char_buf[1] = '\0';
          printc(char_buf, VIDEO_WHITE);

          if (*p != '\0') {
              char_buf[0] = *p;
              char_buf[1] = '\0';
              printc(char_buf, VIDEO_WHITE);
          } else {
              p--;
          }
          break;
        }
      }
    } else {
      char_buf[0] = *p;
      char_buf[1] = '\0';
      printc(char_buf, VIDEO_WHITE);
    }
    if (*p != '\0') {
        p++;
    }
  }
  va_end(args);
}

void clear_line(int line) {
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    volatile char *video = cursorPosToAddress(x, line);
    *video = ' ';
  }
}