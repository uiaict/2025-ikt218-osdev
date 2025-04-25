#ifndef PRINT_H
#define PRINT_H

typedef enum {
  VIDEO_BLACK = 0,
  VIDEO_BLUE = 1,
  VIDEO_GREEN = 2,
  VIDEO_CYAN = 3,
  VIDEO_RED = 4,
  VIDEO_PURPLE = 5,
  VIDEO_BROWN = 6,
  VIDEO_GRAY = 7,
  VIDEO_DARK_GRAY = 8,
  VIDEO_LIGHT_BLUE = 9,
  VIDEO_LIGHT_GREEN = 10,
  VIDEO_LIGHT_CYAN = 11,
  VIDEO_LIGHT_RED = 12,
  VIDEO_LIGHT_PURPLE = 13,
  VIDEO_YELLOW = 14,
  VIDEO_WHITE = 15
} VideoColour;

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern int cursorPositionX_;
extern int cursorPositionY_;

/**
 * @brief Proper print function.
 *
 * Prints like a console.
 */
void print(const char *string);

void printc(const char *string, VideoColour colour);

void incrementCursorPosition();

char *cursorPosToAddress(int x, int y);

void clear_line(int line);

#endif