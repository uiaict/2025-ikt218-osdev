#ifndef MATRIX_H
#define MATRIX_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// Matrix rain animation settings
#define MATRIX_GREEN        0x0A    // Bright green color attribute
#define MATRIX_BRIGHT_GREEN 0x0A    // Bright green color attribute
#define MATRIX_DARK_GREEN   0x02    // Dark green color attribute
#define MATRIX_WHITE        0x0F    // White color attribute

// Structure to track a single raindrop column
typedef struct {
    int length;           // Length of this raindrop
    int position;         // Current y position
    int speed;            // How fast it falls (update every N ticks)
    int tick_counter;     // Tick counter for speed control
    int active;           // Whether this raindrop is currently active
    char chars[25];       // Characters in this raindrop
    uint8_t colors[25];   // Color for each character in the raindrop
} matrix_raindrop_t;

// Initialize and run the Matrix rain animation
void matrix_init();
void matrix_start();
void matrix_stop();
void matrix_update();
uint8_t matrix_rand();
void matrix_restore_screen();

#endif // MATRIX_H
