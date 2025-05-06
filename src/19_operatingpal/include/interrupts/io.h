#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_TEXT_COLOR 0x07
#define DEFAULT_BACKGROUND_COLOR 0x00

// Global variables for VGA text mode output
extern int cursorPos;
extern char* videoMemory;

// Cursor control
void enableCursor(uint8_t cursorStart, uint8_t cursorEnd);
void disableCursor();
void setCursorPosition(uint16_t position);
uint16_t getCursorPosition();

// Screen and text control
void clearScreen();
void scroll();
void changeTextColor(uint8_t color);
void changeBackgroundColor(uint8_t color);

// Low-level I/O ports
void outb(unsigned short port, unsigned char val);
uint8_t inb(uint16_t port);


#endif // IO_H
