#pragma once

typedef enum {
    NO_COMMAND,
    LOAD_INFO,
    LOAD_MUSIC_PLAYER
} ShellCommand_t;

void print_shell_welcome_message();
void print_shell_command_not_found();

ShellCommand_t get_shell_command();