#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// VGA color constants
#define COLOR8_BLACK         0
#define COLOR8_BLUE          1
#define COLOR8_GREEN         2
#define COLOR8_CYAN          3
#define COLOR8_RED           4
#define COLOR8_MAGENTA       5
#define COLOR8_BROWN         6
#define COLOR8_LIGHT_GREY    7
#define COLOR8_DARK_GREY     8
#define COLOR8_LIGHT_BLUE    9
#define COLOR8_LIGHT_GREEN   10
#define COLOR8_LIGHT_CYAN    11
#define COLOR8_LIGHT_RED     12
#define COLOR8_LIGHT_MAGENTA 13
#define COLOR8_LIGHT_BROWN   14 
#define COLOR8_YELLOW        14  // Define YELLOW as the same as LIGHT_BROWN
#define COLOR8_WHITE         15

// Rename to avoid conflicts with function parameters
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

// Function prototypes
void Reset(void);
void setColor(uint8_t fg, uint8_t bg);
void scrollUp(void);
void newLine(void);
void print(const char* text);
void putCharAt(uint16_t x, uint16_t y, char c, uint8_t fg, uint8_t bg);
void setCursorPosition(uint16_t x, uint16_t y);
void getCursorPosition(uint16_t* x, uint16_t* y); 
uint16_t getScreenWidth(void);
uint16_t getScreenHeight(void);

// 4-frame animation
void show_animation(void);

#endif