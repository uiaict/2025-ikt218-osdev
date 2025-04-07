#include "music_player/song_player.h"
#include "interrupts/speaker.h"
#include "terminal/print.h"
#include "memory/heap.h"

#include "libc/stddef.h"

// Play song function
void song_player_func(Song* song) {
    for (size_t i = 0; i < song->note_count; i++) {
        speaker_beep(song->notes[i].frequency, song->notes[i].duration);
    }
}

// Create a new song player
SongPlayer *create_song_player() {
    SongPlayer *player = (SongPlayer *)malloc(sizeof(SongPlayer));
    if (player == NULL) {
        printf("Failed to allocate memory for SongPlayer\n");
        return NULL;
    }
    player->play_song = song_player_func;
    return player;
}

void destroy_song_player(SongPlayer *player) {
    if (player != NULL) {
        free(player);
    }
}