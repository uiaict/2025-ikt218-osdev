#ifndef PCSPKR_H
#define PCSPKR_H

#include "common.h"

// Initialize the PC Speaker
void init_pcspkr();

// Play a tone at the specified frequency (in Hz) for the specified duration (in ms)
void pcspkr_play_tone(u32int frequency, u32int duration);

// Play a note based on musical notation (C4, D4, etc.) for the specified duration (in ms)
void pcspkr_play_note(const char* note, u32int duration);

// Stop playing any sounds (silence the speaker)
void pcspkr_stop();

// Simple beep with default frequency (1000Hz) and duration (100ms)
void pcspkr_beep();

#endif