#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"

#include "libc/stdio.h"
#include "libc/system.h"
#include "monitor.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define COLUMN_SPACING 3         // space between columns (horizontal)
#define TRAIL_LENGTH 6           // vertical trail size
#define RAIN_DELAY_MS 100        // delay between frames (ms)
#define VERTICAL_SKIP 1          // vertical density (1 = full, 2 = every second line)

extern void sleep_interrupt(uint32_t ms); // declare it explicitly

static uint32_t seed = 123456;
uint32_t rand_simple() {
    seed = seed * 1664525 + 1013904223;
    return seed;
}

char random_char() {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$&*";
    return charset[rand_simple() % (sizeof(charset) - 1)];
}


// Colors for the matrix rain effect. The characters are green on a black background.
void draw_matrix_rain() {
    uint8_t color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    monitor_setcolor(color);
    monitor_clear();

    int positions[SCREEN_WIDTH] = {0};
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        positions[i] = rand_simple() % SCREEN_HEIGHT;
    }

    while (true) {
        for (int x = 0; x < SCREEN_WIDTH; x += COLUMN_SPACING) {
            int y = positions[x];

            if (y % VERTICAL_SKIP == 0) {
                monitor_putentryat(random_char(), color, x, y);

                // Wrap around and clear properly
                int clear_y = (y + SCREEN_HEIGHT - TRAIL_LENGTH) % SCREEN_HEIGHT;
                monitor_putentryat(' ', color, x, clear_y);
            }

            positions[x] = (positions[x] + 1) % SCREEN_HEIGHT;
        }

        sleep_interrupt(RAIN_DELAY_MS);
    }
}
