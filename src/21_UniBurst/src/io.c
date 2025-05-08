#include "io.h"
#include "macros.h"

// Video memory begins at address 0xb8000
char* videoMemory = (char*) 0xb8000;


uint8_t currentTextColor = DEFAULT_TEXT_COLOR;
uint8_t currentBackgroundColor = DEFAULT_BACKGROUND_COLOR;
int cursorPos = 0; 


// Clears the screen
void clearScreen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) { 
        videoMemory[i] = ' ';
        videoMemory[i + 1] = currentTextColor; 
        videoMemory[i + 1] |= currentBackgroundColor << 4; 
    }

    cursorPos = 0; 
    setCursorPosition(cursorPos); 
}

// Changes the text color
void changeTextColor(uint8_t color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) { 
        videoMemory[i + 1] = (videoMemory[i + 1] & 0xF0) | (color & 0x0F); 
    }

    currentTextColor = color; 
}

// Changes the background color 
void changeBackgroundColor(uint8_t color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) { 
        videoMemory[i + 1] = (videoMemory[i + 1] & 0x0F) | (color << 4); 
    }
    
    currentBackgroundColor = color; 
}



void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port)); 
}


uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Enables the cursor 
void enableCursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A); 
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start); 
    outb(0x3D4, 0x0B); 
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end); 
}

// Disables the cursor
void disableCursor() {
    outb(0x3D4, 0x0A); 
    outb(0x3D5, 0x20); 
}

// Sets the cursor position
void setCursorPosition(uint16_t pos) {
    outb(0x3D4, 14); 
    outb(0x3D5, pos >> 8);

    outb(0x3D4, 15); 
    outb(0x3D5, pos); 
}


void scroll() {
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1) * 2; i++) { 
        videoMemory[i] = videoMemory[i + VGA_WIDTH * 2]; 
    }

    for (int i = 0; i < VGA_WIDTH * 2; i += 2) { 
        videoMemory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + i] = ' '; 
        videoMemory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + i + 1] = (currentBackgroundColor << 4) | (videoMemory[i + 1] & 0x0F); 
    }
}


