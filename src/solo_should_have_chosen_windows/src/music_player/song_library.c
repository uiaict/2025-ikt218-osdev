#include "music_player/song_library.h"
#include "music_player/song_player.h"
#include "music_player/frequencies.h"

#include "memory/heap.h"
#include "terminal/print.h"
#include "libc/stddef.h"

static Note fur_elise_notes[] = {
    // First phrase
    {E5, 150}, {0, 20},
    {Ds5, 150}, {0, 20},
    {E5, 150}, {0, 20},
    {Ds5, 150}, {0, 20},
    {E5, 150}, {0, 20},
    
    // Second phrase
    {B4, 150}, {0, 20},
    {D5, 150}, {0, 20},
    {C5, 150}, {0, 20},
    
    // Third phrase
    {A4, 150}, {0, 20},
    {C4, 150}, {0, 20},
    {E4, 150}, {0, 20},
    {A4, 150}, {0, 20},
    
    // Final phrase
    {B4, 200}, {0, 20},
    {E4, 150}, {0, 20},
    {A4, 150}, {0, 20},
    {B4, 150}, {0, 20},
    {C5, 500}, {0, 20}
};

static Note twinkle_twinkle_notes[] = {
    {C4, 300}, {0, 20}, {C4, 300}, {0, 20},
    {G4, 300}, {0, 20}, {G4, 300}, {0, 20},
    {A4, 300}, {0, 20}, {A4, 300}, {0, 20},
    {G4, 600}, {0, 20},

    {F4, 300}, {0, 20}, {F4, 300}, {0, 20},
    {E4, 300}, {0, 20}, {E4, 300}, {0, 20},
    {D4, 300}, {0, 20}, {D4, 300}, {0, 20},
    {C4, 600}
};

static Note happy_birthday_notes[] = {
    {C4, 250}, {0, 20}, {C4, 250}, {0, 20}, {D4, 500}, {0, 20},
    {C4, 500}, {0, 20}, {F4, 500}, {0, 20}, {E4, 1000}, {0, 20},

    {C4, 250}, {0, 20}, {C4, 250}, {0, 20}, {D4, 500}, {0, 20},
    {C4, 500}, {0, 20}, {G4, 500}, {0, 20}, {F4, 1000}, {0, 20},

    {C4, 250}, {0, 20}, {C4, 250}, {0, 20}, {C5, 500}, {0, 20},
    {A4, 500}, {0, 20}, {F4, 500}, {0, 20}, {E4, 500}, {0, 20},
    {D4, 1000}, {0, 20},

    {As4, 250}, {0, 20}, {As4, 250}, {0, 20}, {A4, 500}, {0, 20},
    {F4, 500}, {0, 20}, {G4, 500}, {0, 20}, {F4, 1000}
};

Song fur_elise_song = {
    .title = "Fur Elise",
    .artist = "Ludwig van Beethoven",
    .notes = fur_elise_notes,
    .note_count = sizeof(fur_elise_notes) / sizeof(Note)
};

Song twinkle_twinkle_song = {
    .title = "Twinkle Twinkle Little Star",
    .artist = "Traditional",
    .notes = twinkle_twinkle_notes,
    .note_count = sizeof(twinkle_twinkle_notes) / sizeof(Note)
};

Song happy_birthday_song = {
    .title = "Happy Birthday",
    .artist = "Traditional",
    .notes = happy_birthday_notes,
    .note_count = sizeof(happy_birthday_notes) / sizeof(Note)
};

Song* songList = NULL;
size_t numOfSongs = 0;

void init_song_library() {
    numOfSongs = 3;
    songList = (Song*)malloc(numOfSongs * sizeof(Song));
    if (songList == NULL) {
        printf("Failed to allocate memory for song library\n");
        return;
    }

    songList[0] = fur_elise_song;
    songList[1] = twinkle_twinkle_song;
    songList[2] = happy_birthday_song;
}

void destroy_song_library() {
    if (songList != NULL) {
        free(songList);
        songList = NULL;
    }
}