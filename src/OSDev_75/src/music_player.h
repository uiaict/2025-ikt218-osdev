#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "drivers/audio/song.h"
#include "menu.h"  

#define MENU_STATE_MUSIC_PLAYER MENU_STATE_MUSIC

#define TEXT_PLAYER_WIDTH 78
#define TEXT_PLAYER_HEIGHT 20
#define PLAYER_BORDER_COLOR COLOR8_LIGHT_BLUE
#define PLAYER_TITLE_COLOR COLOR8_CYAN
#define PLAYER_TEXT_COLOR COLOR8_WHITE
#define PLAYER_HIGHLIGHT_COLOR COLOR8_YELLOW
#define PLAYER_BG_COLOR COLOR8_BLACK

#define NUM_SONGS 6

void init_music_player();
void music_player_loop();
void draw_player_frame();
void handle_player_input();
void update_player();

extern bool is_playing;
extern uint8_t current_song;
extern uint16_t current_note;
extern uint32_t note_start_time;
extern const char* song_names[NUM_SONGS];
extern Song songs[NUM_SONGS];

#endif // MUSIC_PLAYER_H