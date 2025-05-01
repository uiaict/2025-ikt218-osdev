#include "libc/matrix_rain.h"
#include "libc/stdint.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define VIDEO_MEMORY ((uint16_t*)0xB8000)

static uint32_t rand_seed = 12345678;

// Simple linear congruential generator (LCG)
uint32_t rand() {
    rand_seed = rand_seed * 1664525 + 1013904223;
    return rand_seed;
}

// Return a printable ASCII character (from 33 to 126)
char random_char() {
    return 33 + (rand() % 94);
}

// Crude delay using busy loop
void delay() {
    for (volatile int i = 0; i < 100000; i++);
}

// Write character with color at (x, y)
void put_char(int x, int y, char c, uint8_t color) {
    VIDEO_MEMORY[y * SCREEN_WIDTH + x] = (color << 8) | c;
}

// Clear screen by writing spaces with black color
void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            put_char(x, y, ' ', 0x00);
        }
    }
}

void matrix_rain() {
    int positions[SCREEN_WIDTH];

    // Initialize column positions randomly
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        positions[i] = rand() % SCREEN_HEIGHT;
    }

    clear_screen();

    while (1) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int y = positions[x];

            // Erase the previous character
            if (y > 0) {
                put_char(x, y - 1, ' ', 0x00);
            }

            // Print new random character
            char c = random_char();
            put_char(x, y, c, 0x0A); // Bright green

            // Move head down or wrap around
            positions[x] = (positions[x] + 1) % SCREEN_HEIGHT;
        }

        delay();  // crude animation delay
    }
}
