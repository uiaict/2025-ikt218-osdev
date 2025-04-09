#pragma once

typedef enum {
    LOAD_INFO,
    LOAD_MUSIC_PLAYER
} ShellCommand_t;


ShellCommand_t get_shell_command(char *command);
void get_last_line();