#include "music_player/song_player.h"
#include "music_player/song_library.h"


#include "interrupts/pit.h"
#include "interrupts/speaker.h"
#include "terminal/print.h"
#include "memory/heap.h"

#include "libc/stdint.h"
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

void playAllSongs(Song *songs, size_t song_count) {
    SongPlayer *player = create_song_player();
    if (player == NULL) {
        printf("Failed to create song player\n");
        return;
    }

    printf("Heap after player creation:\n");
    print_heap();
    
    for (size_t i = 0; i < song_count; i++) {
            printf("Playing song %u: %s by %s\n", (unsigned int) i + 1, songs[i].title, songs[i].artist);
            player->play_song(&songs[i]);
            sleep_busy(1000);
            if (i < song_count - 1) {
                printf("Next song...\n");
            }
            else {
                printf("End of playlist.\n");
            }
    }

    destroy_song_player(player);
}