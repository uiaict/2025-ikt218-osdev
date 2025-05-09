#pragma once

typedef enum {
    NO_COMMAND,
    CLEAR_SCREEN,
    LOAD_STATIC_SCREEN,
    LOAD_MUSIC_PLAYER,
    LOAD_ART,
    HEAP_PRINT
} ShellCommand_t;

typedef enum {
    NO_MUSIC_COMMAND,
    LOAD_MUSIC_PLAYER_HELP,
    EXIT,
    CLEAR_SCREEN_MUSIC,
    PLAY_SONG,
    LIST_SONGS,
    SHOW_INFO
} Music_Command_t;

typedef enum {
    NO_ART_COMMAND,
    LOAD_ART_HELP,
    CLEAR_SCREEN_ART,
    NEW_DRAWING,
    LOAD_DRAWING,
    LIST_DRAWINGS,
    DELETE_DRAWING,
    ART_EXIT
} Art_Command_t;

// Shell stubs
extern const char* launch_stub;
extern const char* info_stub;
extern const char* help_stub;
extern const char* clear_stub;
extern const char* heap_print_stub;

// Music stubs
extern const char* music_player_stub;
extern const char* music_command_stub;
extern const char* music_command_play;
extern const char* music_command_list;
extern const char* music_command_exit;
extern const char* music_command_info;

// Art stubs
extern const char* art_launch_stub;
extern const char* art_command_list;
extern const char* art_command_exit;
extern const char* art_command_new;
extern const char* art_command_load;
extern const char* art_command_delete;


ShellCommand_t get_shell_command();
char* get_shell_command_string();

Music_Command_t get_music_command();
char* get_music_command_string(Music_Command_t cmd);

Art_Command_t get_art_command();
char* get_art_command_string(Art_Command_t cmd);