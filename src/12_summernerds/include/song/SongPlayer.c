#include <song/song.h>
#include "../include/kernel/pit.h"
#include <libc/stdio.h>

void enable_speaker() {
    // Pseudocode for enable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. The control register bits are usually defined as follows:
    //    - Bit 0 (Speaker gate): Controls whether the speaker is on or off.
    //    - Bit 1 (Speaker data): Determines if data is being sent to the speaker.
    // 3. Set both Bit 0 and Bit 1 to enable the speaker.
    //    - Use bitwise OR operation to set these bits without altering others.
}

void disable_speaker() {
    // Pseudocode for disable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. Clear both Bit 0 and Bit 1 to disable the speaker.
    //    - Use bitwise AND with the complement of 3 (0b11) to clear these bits.
}

void play_sound(uint32_t frequency) {
    // Pseudocode for play_sound:
    // 1. Check if the frequency is 0. If so, exit the function as this indicates no sound.
    // 2. Calculate the divisor for setting the PIT (Programmable Interval Timer) frequency.
    //    - The PIT frequency is a base value, typically 1.193182 MHz.
    //    - The divisor is PIT frequency divided by the desired sound frequency.
    // 3. Configure the PIT to the desired frequency:
    //    - Send control word to PIT control port to set binary counting, mode 3, and access mode (low/high byte).
    //    - Split the calculated divisor into low and high bytes.
    //    - Send the low byte followed by the high byte to the PIT channel 2 port.
    // 4. Enable the speaker (by setting the appropriate bits) to start sound generation.
}

void stop_sound() {
    // Pseudocode for stop_sound:
    // 1. Read the current state from the PC speaker control port.
    // 2. Clear the bit that enables speaker data to stop the sound.
    //    - Use bitwise AND with the complement of the bit responsible for enabling the speaker data.
}

void play_song_impl(Song *song) {
    // Pseudocode for play_song_impl:
    // 1. Enable the speaker before starting the song.
    // 2. Loop through each note in the song's notes array:
    //    a. For each note, display its details such as frequency and duration.
    //    b. Call play_sound with the note's frequency.
    //    c. Delay execution for the duration of the note (this can be implemented with a sleep function).
    //    d. Call stop_sound to end the note.
    // 3. Disable the speaker after all notes have been played.
}

void play_song(Song *song) {
    // Pseudocode for play_song:
    // 1. Call play_song_impl with the given song.
    //    - This function handles the entire process of playing each note in the song.
}