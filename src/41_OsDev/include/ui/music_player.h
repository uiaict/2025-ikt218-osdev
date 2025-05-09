#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <driver/include/keyboard.h>

////////////////////////////////////////
// Configuration
////////////////////////////////////////

#define MAX_SONGS 16

////////////////////////////////////////
// Note and SongEntry Structures
////////////////////////////////////////

// Single note (0 Hz = rest)
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

// Metadata for a song
typedef struct {
    char title[30];
    Note* notes;
    uint32_t length;
} SongEntry;

////////////////////////////////////////
// Music Player State
////////////////////////////////////////

typedef struct {
    SongEntry songs[MAX_SONGS];
    uint32_t song_count;
    uint32_t selected_index;
    bool is_playing;
    bool running;
} MusicPlayer;

////////////////////////////////////////
// Music Player API
////////////////////////////////////////

void music_player_init(MusicPlayer* player);
void music_player_add_song(MusicPlayer* player, const char* title, Note* notes, uint32_t length);
void music_player_render(MusicPlayer* player);
void music_player_play_selected(MusicPlayer* player);
void music_player_handle_input(MusicPlayer* player, uint8_t key);
void music_player_run(MusicPlayer* player);
void music_player_exit(MusicPlayer* player);
void launch_music_player(void);

#endif // MUSIC_PLAYER_H
