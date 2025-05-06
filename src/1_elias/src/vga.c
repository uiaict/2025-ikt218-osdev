#include "libc/vga.h"

uint16_t column = 0;
uint16_t row = 0;
uint16_t* const video_memory = (uint16_t* const) 0xB8000;
const uint16_t defaultColor = (COLOR8_BLACK << 12) |
(COLOR8_LIGHT_GREY << 8);
uint16_t currentColor = defaultColor;

void Reset() {
    row = 0;
    column = 0;
    currentColor = defaultColor;

    for( uint16_t y = 0; y < vgaHeight; y++) {
        for( uint16_t x = 0; x < vgaWidth; x++) {
            video_memory[y * vgaWidth + x] = ' ' | defaultColor;
        }
    }
}

void newLine() {
    if( row < vgaHeight - 1) {
        row++;
        column = 0;
    }
    else {
        scrollUp();
        column = 0;
    }
}

void scrollUp() {
    for (uint16_t y = 0; y < vgaHeight; y++) {
        for (uint16_t x = 0; x < vgaWidth; x++) {
            video_memory[(y-1) * vgaWidth + x] = video_memory[y * vgaWidth + x];
        }
    }

    for (uint16_t x = 0; x < vgaWidth; x++) {
        video_memory[(vgaHeight-1) * vgaWidth + x] = ' ' |
        currentColor;
    }
}

void print(const char* s) {
    while(*s) {
        switch(*s) {
            case '\n':
                newLine();
                break;
            case '\r':
                column = 0;
                break;
            case '\t':
                if(column == vgaWidth) {
                    newLine();
                }
                uint16_t tabLen = 4 - (column % 4);
                while (tabLen != 0) {
                    video_memory[row * vgaWidth + (column++)] = ' ' |
                    currentColor;
                    tabLen--;
                }
                break;
            default:
                if( column == vgaWidth) {
                    newLine();
                }

                video_memory[row * vgaWidth + (column++)] = *s |
                currentColor;
                break;
        }
        s++;
    }
}