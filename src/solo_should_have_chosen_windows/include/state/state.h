#pragma once

typedef enum {
    START_SCREEN,
    SHELL,
    STATIC_SCREEN,
    MUSIC_PLAYER,
    MUSIC_PLAYER_HELP,
    SONG_PLAYING,
    ASCII_ART_BOARD_MENU,
    WHOLE_KEYBOARD,
    NOT_USED
} SystemState;


void update_state(void);
SystemState get_current_state(void);
void change_state(SystemState new_state);