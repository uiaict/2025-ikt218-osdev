#ifndef MATRIX_RAIN_H
#define MATRIX_RAIN_H

#include "libc/stdint.h"
/**
 * Initializes and runs the Matrix Rain animation.
 * This function loops indefinitely and should be called
 * from the kernel main or a menu handler.
 */
void run_matrix_rain(void);

/**
 * Performs a single step of the Matrix Rain animation.
 * Typically called repeatedly with a delay (e.g., using PIT).
 */
void matrix_rain_step(void);

/**
 * Simple pseudo-random number generator returning a byte.
 * Can be used to add randomness to rain drop locations.
 */
uint8_t rand_byte(void);
void clear_screen();

#endif // MATRIX_RAIN_H
