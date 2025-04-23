#ifndef COMMAND_H
#define COMMAND_H

#include <libc/stdbool.h>
#include "song.h"

// Maximum command length
#define MAX_COMMAND_LENGTH 64

// Command handler function
void process_command(const char* cmd);

// Command buffer utilities
void init_command_buffer();
void append_to_command(char c);
void execute_current_command();
void clear_command_buffer();

// External declarations
extern char command_buffer[MAX_COMMAND_LENGTH];
extern int command_length;
extern const struct note mario_theme[];

#endif // COMMAND_H
