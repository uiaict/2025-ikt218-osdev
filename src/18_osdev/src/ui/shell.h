#ifndef SHELL_H
#define SHELL_H

#include "libc/string.h"
#include "libc/monitor.h"
#include "../keyboard/keyboard.h"
#include "../song/song.h" // Include your songs header

// Initialize and run the shell
void init_shell();

// Main shell loop - returns when user quits
void run_shell();

// Process a single command
void process_command(char* command);

#endif