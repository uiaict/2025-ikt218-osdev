#pragma once

typedef enum {
    NO_COMMAND,
    LOAD_STATIC_SCREEN,
    LOAD_MUSIC_PLAYER
} ShellCommand_t;

void print_shell_command_not_found();

ShellCommand_t get_shell_command();
char* get_shell_command_string();