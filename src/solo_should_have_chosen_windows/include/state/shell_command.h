#pragma once

typedef enum {
    NO_COMMAND,
    CLEAR_SCREEN,
    LOAD_STATIC_SCREEN,
    LOAD_MUSIC_PLAYER
} ShellCommand_t;

extern const char* launch_stub;
extern const char* info_stub;
extern const char* help_stub;
extern const char* clear_stub;

void print_shell_command_not_found();

ShellCommand_t get_shell_command();
char* get_shell_command_string();