#include <vga.h>

uint16_t coloumn = 0;
uint16_t line = 0;
uint16_t* const vga = (uint16_t* const) 0xB8000;
const uint16_t default_colour = (COLOUR_WHITE << 8) | (COLOUR_BLACK << 12);
uint16_t current_colour = default_colour;

void Reset() {
    line = 0;
    coloumn = 0;
    current_colour = default_colour;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[y * width + x] = ' ' | default_colour;
        }
    }
}

void newLine() {
    if (line < height -1) {
        line++;
        coloumn = 0;
    } else {
        scrollup();
        coloumn = 0;
    }
}

void scrollup() {
    // Copy each line to the line above it (starting from the second line)
    for (uint16_t y = 1; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            vga[(y-1) * width + x] = vga[y * width + x];
        }  
    }
    // Clear the last line
    for (uint16_t x = 0; x < width; x++) {
        vga[(height-1) * width + x] = ' ' | current_colour;
    }
}

void print(const char* s, int colour) {
    if (colour != 0) {
        current_colour = (colour << 8) | (COLOUR_BLACK << 12);
    } else {
        current_colour = default_colour;
    }
    while (*s){
        switch(*s){
            case '\n':
                newLine();
                break;
            case '\r':
                coloumn = 0;
                break;
            case '\t':
                if (coloumn == width) {
                    newLine();
                }
                uint16_t tabLen = 4 - (coloumn % 4);
                while (tabLen != 0) {
                    vga[line * width + (coloumn++)] = ' ' | current_colour;
                    tabLen--;
                }
                break;
            default:
                if (coloumn == width) {
                    newLine();
                }
                vga[line * width + (coloumn++)] = *s | current_colour;
                break;
        }
        s++;
    }
}


