#include "libc/matrix_rain.h"
#include "libc/stdint.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define VIDEO_MEMORY ((uint16_t*)0xB8000)

static uint32_t rand_seed = 12345678;
static int positions[SCREEN_WIDTH];
static int initialized = 0;

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

void matrix_rain_init() {
    clear_screen();
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        positions[i] = rand() % SCREEN_HEIGHT;
    }
    initialized = 1;
}

void matrix_rain_tick() {
    if (!initialized) matrix_rain_init();

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        int y = positions[x];

        if (y > 0) {
            put_char(x, y - 1, ' ', 0x00); // Erase previous char
        }

        char c = random_char();
        put_char(x, y, c, 0x0A); // Green

        positions[x] = (positions[x] + 1) % SCREEN_HEIGHT;
    }
}
#include "libc/system.h"  // for sleep_interrupt
#include "libc/stdio.h"   // for printf

void matrix_rain_intro(uint32_t frames, uint32_t delay_ms) {
    clear_screen();
    printf("Welcome to our OS! We are group 20!\n");
    sleep_interrupt(1000);  // pause for dramatic effect

    for (uint32_t i = 0; i < frames; i++) {
        matrix_rain_tick();
        sleep_interrupt(delay_ms);
    }

    clear_screen();
    printf("Starting system...\n");
    sleep_interrupt(500);
}

