#include "libc/stdint.h"
#include "monitor.h"
#include "common.h"


static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 

uint16_t *videoMemory = (uint16_t *)0xB8000;    // Memory address of video memory
size_t terminalRow;                             // Represents which row is being writen to
size_t terminalColumn;                          // Represents which colum is being writen to
uint8_t terminalColor;                          // The first 4 bit represents the text color and the latter 4 represent the background color
uint16_t* terminalBuffer;

// Initializes variables and clears the screen
void monitorInitialize() {
    terminalRow = 0;
    terminalColumn = 0;
    terminalColor = (15 | 0 << 4);      // Makes the text white (15) and background black (0) by leftshifting 0 4 steps
    terminalBuffer = videoMemory;

    // Fills the screen with the right color and spaces
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            // Writes ' ' with the color terminalColor to the correct place in the video memory
            terminalBuffer[y * VGA_WIDTH + x] = (uint16_t) ' ' | (uint16_t) terminalColor << 8;
        }
    }
}

// "scrolles" the screen when terminalRow >= VGA_HEIGHT
static void scroll()
{
    // Space with terminalColor
    uint16_t blank = 0x20 | (terminalColor << 8);

    if(terminalRow >= VGA_HEIGHT)
    {
        size_t i;
        // Moves every line upp one line
        for (i = 0; i < (VGA_HEIGHT-1)*VGA_WIDTH; i++)
        {
            terminalBuffer[i] = terminalBuffer[i+VGA_WIDTH];
        }

        // replaces the bottom row with spaces
        for (i = (VGA_HEIGHT-1)*VGA_WIDTH; i < VGA_HEIGHT*VGA_WIDTH; i++)
        {
            terminalBuffer[i] = blank;
        }
        terminalRow = VGA_HEIGHT-1;
    }
}

// Moves the coursor in relation to the last caracter
static void moveCursor()
{
    uint16_t pos = terminalRow * VGA_WIDTH + terminalColumn;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// takes in a char and places it in the correct location
void monitorPut(char c) {
    // Moves to the next line if \n is detected
    switch (c)
    {
        case '\n':
            terminalRow++;
            terminalColumn = 0;
            return;
        default:
            break;
    }

	const size_t index = terminalRow * VGA_WIDTH + terminalColumn;
    terminalBuffer[index] = (uint16_t) c | (uint16_t) terminalColor << 8;
    // Moves to the next line when the current line is full
    if (++terminalColumn == VGA_WIDTH){
        terminalColumn = 0;
        terminalRow++;
    }

    moveCursor();
    scroll();

}