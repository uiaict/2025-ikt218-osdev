//
// vga.c
//
// Implements basic text-mode printing and a simple two-frame ASCII animation.
//

#include "vga.h"

// VGA buffer address for text mode
static uint16_t* const VGA_BUFFER = (uint16_t* const)0xB8000;

// Cursor position
static uint16_t column = 0;
static uint16_t line   = 0;

// Default color: light grey on black
static const uint16_t defaultC = (COLOR8_LIGHT_GREY << 8) | (COLOR8_BLACK << 12);
static uint16_t currentColor   = 0;

// Set the current color bits
void setColor(uint8_t fg, uint8_t bg) {
    // Our scheme: bits [15..12]=bg, bits [11..8]=fg
    currentColor = ((uint16_t)fg << 8) | ((uint16_t)bg << 12);
}

// Clears the screen using currentColor
void Reset(void) {
    line = 0;
    column = 0;

    // Revert to default color on each Reset
    currentColor = defaultC;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            VGA_BUFFER[y * width + x] = (uint16_t)' ' | defaultC;
        }
    }
}

// Helper to move to next line or scroll
void newLine(void) {
    if (line < height - 1) {
        line++;
        column = 0;
    } else {
        scrollUp();
        column = 0;
    }
}

// Scroll up by one line
void scrollUp(void) {
    for (uint16_t y = 1; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            VGA_BUFFER[(y - 1) * width + x] = VGA_BUFFER[y * width + x];
        }
    }
    // Blank out the last line
    for (uint16_t x = 0; x < width; x++) {
        VGA_BUFFER[(height - 1) * width + x] = (uint16_t)' ' | currentColor;
    }
}

// Basic text print
void print(const char* s) {
    while (*s) {
        if (*s == '\n') {
            newLine();
        } else if (*s == '\r') {
            column = 0;
        } else if (*s == '\b') {
            if (column > 0) {
                column--;
                VGA_BUFFER[line * width + column] = ' ' | currentColor;
            }
        } else {
            if (column == width) {
                newLine();
            }
            VGA_BUFFER[line * width + column] = (uint16_t)*s | currentColor;
            column++;
        }
        s++;
    }
}

// A crude spin-loop delay so animation is visible
static void delay(unsigned long count) {
    for (volatile unsigned long i = 0; i < count; i++) {
        __asm__ __volatile__("nop");
    }
}

// Two frames: first white, then red. Same ASCII text in each.
static const char* FRAMES[2] = {
    // FRAME 0: White text
    " /$$   /$$ /$$$$$$  /$$$$$$ \n"
    "| $$  | $$|_  $$_/ /$$__  $$\n"
    "| $$  | $$  | $$  | $$  \\ $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$  | $$  | $$  | $$__  $$\n"
    "| $$  | $$  | $$  | $$  | $$\n"
    "|  $$$$$$/ /$$$$$$| $$  | $$\n"
    " \\______/ |______/|__/  |__/\n"
    "\n"
    "$$\\   $$\\ $$$$$$\\  $$$$$$\\  \n"
    "$$ |  $$ |\\_$$  _|$$  __$$\\ \n"
    "$$ |  $$ |  $$ |  $$ /  $$ |\n"
    "$$ |  $$ |  $$ |  $$$$$$$$ |\n"
    "$$ |  $$ |  $$ |  $$  __$$ |\n"
    "$$ |  $$ |  $$ |  $$ |  $$ |\n"
    "\\$$$$$$  |$$$$$$\\ $$ |  $$ |\n"
    " \\______/ \\______/\\__|  \\__|\n"
    "\n"
    " __    __  ______   ______  \n"
    "|  \\  |  \\|      \\ /      \\ \n"
    "| $$  | $$ \\$$$$$$|  $$$$$$\\\n"
    "| $$  | $$  | $$  | $$__| $$\n"
    "| $$  | $$  | $$  | $$    $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$__/ $$ _| $$_ | $$  | $$\n"
    " \\$$    $$|   $$ \\| $$  | $$\n"
    "  \\$$$$$$  \\$$$$$$ \\$$   \\$$\n"
    "\n"
    " __    __  ______   ______  \n"
    "/  |  /  |/      | /      \\ \n"
    "$$ |  $$ |$$$$$$/ /$$$$$$  |\n"
    "$$ |  $$ |  $$ |  $$ |__$$ |\n"
    "$$ |  $$ |  $$ |  $$    $$ |\n"
    "$$ |  $$ |  $$ |  $$$$$$$$ |\n"
    "$$ \\__$$ | _$$ |_ $$ |  $$ |\n"
    "$$    $$/ / $$   |$$ |  $$ |\n"
    " $$$$$$/  $$$$$$/ $$/   $$/ \n"
    "\n",

    // FRAME 1: Red text
    " /$$   /$$ /$$$$$$  /$$$$$$ \n"
    "| $$  | $$|_  $$_/ /$$__  $$\n"
    "| $$  | $$  | $$  | $$  \\ $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$  | $$  | $$  | $$__  $$\n"
    "| $$  | $$  | $$  | $$  | $$\n"
    "|  $$$$$$/ /$$$$$$| $$  | $$\n"
    " \\______/ |______/|__/  |__/\n"
    "\n"
    "$$\\   $$\\ $$$$$$\\  $$$$$$\\  \n"
    "$$ |  $$ |\\_$$  _|$$  __$$\\ \n"
    "$$ |  $$ |  $$ |  $$ /  $$ |\n"
    "$$ |  $$ |  $$ |  $$$$$$$$ |\n"
    "$$ |  $$ |  $$ |  $$  __$$ |\n"
    "$$ |  $$ |  $$ |  $$ |  $$ |\n"
    "\\$$$$$$  |$$$$$$\\ $$ |  $$ |\n"
    " \\______/ \\______/\\__|  \\__|\n"
    "\n"
    " __    __  ______   ______  \n"
    "|  \\  |  \\|      \\ /      \\ \n"
    "| $$  | $$ \\$$$$$$|  $$$$$$\\\n"
    "| $$  | $$  | $$  | $$__| $$\n"
    "| $$  | $$  | $$  | $$    $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$__/ $$ _| $$_ | $$  | $$\n"
    " \\$$    $$|   $$ \\| $$  | $$\n"
    "  \\$$$$$$  \\$$$$$$ \\$$   \\$$\n"
    "\n"
    " __    __  ______   ______  \n"
    "/  |  /  |/      | /      \\ \n"
    "$$ |  $$ |$$$$$$/ /$$$$$$  |\n"
    "$$ |  $$ |  $$ |  $$ |__$$ |\n"
    "$$ |  $$ |  $$ |  $$    $$ |\n"
    "$$ |  $$ |  $$ |  $$$$$$$$ |\n"
    "$$ \\__$$ | _$$ |_ $$ |  $$ |\n"
    "$$    $$/ / $$   |$$ |  $$ |\n"
    " $$$$$$/  $$$$$$/ $$/   $$/ \n"
    "\n"
};

#define NUM_FRAMES (sizeof(FRAMES)/sizeof(FRAMES[0]))

// Show two frames: White, then Red
void show_animation(void) {
    // Frame 0: set color white on black, clear, print
    setColor(COLOR8_WHITE, COLOR8_BLACK);
    Reset();
    print(FRAMES[0]);
    delay(200000000);

    // Frame 1: set color red on black, clear, print
    setColor(COLOR8_RED, COLOR8_BLACK);
    Reset();
    print(FRAMES[1]);
    delay(200000000);

    // Finally revert to default color (light grey) and clear
    setColor(COLOR8_LIGHT_GREY, COLOR8_BLACK);
    Reset();
}
