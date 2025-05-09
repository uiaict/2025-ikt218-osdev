#include "effects/matrix.h"
#include "kernel/pit.h"
#include "printing/terminal.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "kernel/memory.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

static int drop_positions[SCREEN_WIDTH];
static bool active_columns[SCREEN_WIDTH];


// Creates our characters
char random_char() {
    char c = (rand() % 94) + 33;
    return c;
}

// Gets rid of screen clutter
static void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            terminal_putchar_at(' ', x, y);
        }
    }
}

void run_matrix() {
    clear_screen();

    // Track positions and activity for each column
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        drop_positions[i] = 0;
        active_columns[i] = false;
    }

    while (true) {
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            // 10% chance to activate a new drop in an inactive column
            if (!active_columns[i] && (rand() % 100) < 10) {
                active_columns[i] = true;
                drop_positions[i] = 0;
            }

            if (!active_columns[i])
                continue;

            int y = drop_positions[i];

            char c = random_char();
            terminal_putchar_color_at(c, i, y, 0x0A);

            int tail_length = 5;
            int tail_y = (y - tail_length + SCREEN_HEIGHT) % SCREEN_HEIGHT;
            terminal_putchar_color_at(' ', i, tail_y, 0x0A);

            drop_positions[i] = (y + 1) % SCREEN_HEIGHT;

            if (drop_positions[i] == 0) {
                active_columns[i] = false;
            }
        }

        sleep_interrupt(100);
    }
}

