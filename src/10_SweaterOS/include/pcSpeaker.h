#ifndef PC_SPEAKER_H
#define PC_SPEAKER_H

#include "libc/stdint.h"

// PC Speaker ports
#define PC_SPEAKER_PORT 0x61
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42

// PIT frequency constants
#define PIT_BASE_FREQUENCY 1193180

/**
 * Enable the PC speaker
 * This function enables the PC speaker by setting appropriate bits in the control port
 */
void enable_speaker(void);

/**
 * Disable the PC speaker
 * This function disables the PC speaker by clearing appropriate bits in the control port
 */
void disable_speaker(void);

/**
 * Play a sound at the specified frequency
 * @param frequency The frequency in Hz to play
 */
void play_sound(uint32_t frequency);

/**
 * Stop playing sound
 * This function stops any currently playing sound
 */
void stop_sound(void);

/**
 * Calculate PIT divisor for a given frequency
 * @param frequency The desired frequency in Hz
 * @return The divisor value to use with PIT
 */
uint32_t calculate_pit_divisor(uint32_t frequency);

#endif /* PC_SPEAKER_H */ 