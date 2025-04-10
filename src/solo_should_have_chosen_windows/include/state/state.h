#pragma once

typedef enum {
    START_SCREEN,
    SHELL,
    STATIC_SCREEN,
    MUSIC_PLAYER,
    MUSIC_PLAYER_HELP,
    SONG_PLAYING,
    ART,
    ART_HELP,
    ART_DRAWING,
    WHOLE_KEYBOARD,
    NOT_USED
} SystemState;


void update_state(void);
SystemState get_current_state(void);
void change_state(SystemState new_state);