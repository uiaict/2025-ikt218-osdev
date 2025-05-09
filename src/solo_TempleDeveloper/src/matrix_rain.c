#include "libc/stdint.h"
#include "libc/stdio.h"
#include "pit.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define COLOR_GREEN_ON_BLACK 0x02

// --- Simple Random Generator ---
static uint32_t rand_seed = 123456789;

void srand(uint32_t seed) {
    rand_seed = seed;
}

int rand() {
    rand_seed = rand_seed * 1103515245 + 12345;
    return (rand_seed >> 16) & 0x7FFF;
}

// --- Generate a random character for the matrix rain effect ---
char random_char() {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int index = rand() % (sizeof(charset) - 1);
    return charset[index];
}

// --- Clear screen with blank characters in green ---
void clear_screen() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            put_entry_at(' ', COLOR_GREEN_ON_BLACK, x, y);
        }
    }
}

// --- Matrix rain animation ---
void run_matrix_rain() {
    srand(42); // Deterministic seed

    int column_positions[VGA_WIDTH] = {0};

    clear_screen();

    while (1) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            if (rand() % 10 == 0) {
                column_positions[x] = (column_positions[x] + 1) % VGA_HEIGHT;
                char c = random_char();
                put_entry_at(c, COLOR_GREEN_ON_BLACK, x, column_positions[x]);
            }
        }

        sleep_busy(10);  // Bruk systemets egen forsinkelsesfunksjon
    }
}
