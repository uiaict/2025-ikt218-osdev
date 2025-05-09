#ifndef SHELL_H
#define SHELL_H

#include "libc/string.h"
#include "libc/monitor.h"
#include "libc/stdbool.h"
#include "uiElements.h"
#include "../keyboard/keyboard.h"
#include "../song/song.h" // Include your songs header
#include "../piano/piano.h"
#include "../song/SongPlayer.h"
#include "../game/game.h"

// Initialize and run the shell
void init_shell();

// Main shell loop - returns when user quits
void run_shell();

// Process a single command
void process_command(char* command);

extern volatile bool stop_song_requested;
extern volatile int color;

// Function to check if a key has been pressed while playing
bool should_stop_song();

// Reset the stop flag (call before starting a new song)
void reset_stop_flag();

#endif