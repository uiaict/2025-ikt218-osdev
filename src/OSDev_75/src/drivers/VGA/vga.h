
#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define COLOR8_BLACK        0
#define COLOR8_BLUE         1
#define COLOR8_GREEN        2
#define COLOR8_CYAN         3
#define COLOR8_RED          4
#define COLOR8_MAGENTA      5
#define COLOR8_BROWN        6
#define COLOR8_LIGHT_GREY   7
#define COLOR8_DARK_GREY    8
#define COLOR8_LIGHT_BLUE   9
#define COLOR8_LIGHT_GREEN  10
#define COLOR8_LIGHT_CYAN   11
#define COLOR8_LIGHT_RED    12
#define COLOR8_LIGHT_MAGENTA 13
#define COLOR8_LIGHT_BROWN  14
#define COLOR8_WHITE        15

#define width  80
#define height 25

// VGA routines
void setColor(uint8_t fg, uint8_t bg);
void Reset(void);
void newLine(void);
void scrollUp(void);
void print(const char* s);

// ASCII animation function
void show_animation(void);

#endif
