#pragma once

typedef enum {
    START_SCREEN,
    MENU,
    INFO_SCREEN,
    MUSIC_PLAYER,
    SONG_PLAYING,
    WHOLE_KEYBOARD,
    NOT_USED
} SystemState;


void update_state(void);
SystemState get_current_state(void);
void change_state(SystemState new_state);