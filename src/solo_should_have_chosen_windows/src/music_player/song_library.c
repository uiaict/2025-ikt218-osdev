#include "music_player/song_library.h"
#include "music_player/song_player.h"
#include "music_player/frequencies.h"

#include "memory/heap.h"
#include "terminal/print.h"
#include "libc/stddef.h"
#include "libc/string.h"

static Note fur_elise_notes[] = {
    {E5, 100}, {0, 10},
    {Ds5, 100}, {0, 10},
    {E5, 100}, {0, 10},
    {Ds5, 100}, {0, 10},
    {E5, 100}, {0, 10},

    {B4, 100}, {0, 10},
    {D5, 100}, {0, 10},
    {C5, 100}, {0, 10},

    {A4, 100}, {0, 10},
    {C4, 100}, {0, 10},
    {E4, 100}, {0, 10},
    {A4, 100}, {0, 10},

    {B4, 140}, {0, 10},
    {E4, 100}, {0, 10},
    {A4, 100}, {0, 10},
    {B4, 100}, {0, 10},
    {C5, 300}, {0, 10}
};

static Note twinkle_twinkle_notes[] = {
    {C4, 200}, {0, 10}, {C4, 200}, {0, 10},
    {G4, 200}, {0, 10}, {G4, 200}, {0, 10},
    {A4, 200}, {0, 10}, {A4, 200}, {0, 10},
    {G4, 400}, {0, 10},

    {F4, 200}, {0, 10}, {F4, 200}, {0, 10},
    {E4, 200}, {0, 10}, {E4, 200}, {0, 10},
    {D4, 200}, {0, 10}, {D4, 200}, {0, 10},
    {C4, 400}
};

static Note happy_birthday_notes[] = {
    {C4, 170}, {0, 10}, {C4, 170}, {0, 10}, {D4, 340}, {0, 10},
    {C4, 340}, {0, 10}, {F4, 340}, {0, 10}, {E4, 680}, {0, 10},

    {C4, 170}, {0, 10}, {C4, 170}, {0, 10}, {D4, 340}, {0, 10},
    {C4, 340}, {0, 10}, {G4, 340}, {0, 10}, {F4, 680}, {0, 10},

    {C4, 170}, {0, 10}, {C4, 170}, {0, 10}, {C5, 340}, {0, 10},
    {A4, 340}, {0, 10}, {F4, 340}, {0, 10}, {E4, 340}, {0, 10},
    {D4, 680}, {0, 10},

    {As4, 170}, {0, 10}, {As4, 170}, {0, 10}, {A4, 340}, {0, 10},
    {F4, 340}, {0, 10}, {G4, 340}, {0, 10}, {F4, 680}
};

static Note super_mario_notes[] = {
    {E5, 100}, {0, 30}, {E5, 100}, {0, 100},
    {E5, 100}, {0, 100}, {C5, 100}, {0, 30},
    {E5, 100}, {0, 100}, {G5, 100}, {0, 230},
    {G4, 100}, {0, 230},

    {C5, 100}, {0, 170}, {G4, 100}, {0, 170},
    {E4, 100}, {0, 170}, {A4, 100}, {0, 30},
    {B4, 100}, {0, 30}, {As4, 100}, {0, 30},
    {A4, 100}, {0, 100},

    {G4, 70}, {E5, 70}, {G5, 70}, {A5, 100}, {F5, 100},
    {G5, 100}, {0, 100}, {E5, 100}, {0, 100},
    {C5, 100}, {D5, 100}, {B4, 100}, {0, 200}
};

Song fur_elise_song = {
    .code = "fur_elise",
    .title = "Fur Elise",
    .artist = "Ludwig van Beethoven",
    .notes = fur_elise_notes,
    .information = "Fur Elise is a popular piece of classical music composed by Ludwig van Beethoven. It is often played on the piano and is known for its beautiful melody.",
    .note_count = sizeof(fur_elise_notes) / sizeof(Note)
};

Song twinkle_twinkle_song = {
    .code = "twinkle_twinkle",
    .title = "Twinkle Twinkle Little Star",
    .artist = "Traditional",
    .notes = twinkle_twinkle_notes,
    .information = "Twinkle, Twinkle, Little Star is a popular English lullaby. The melody is from a French tune, 'Ah! vous dirai-je, Maman', which was published in 1761.",
    .note_count = sizeof(twinkle_twinkle_notes) / sizeof(Note)
};

Song happy_birthday_song = {
    .code = "happy_birthday",
    .title = "Happy Birthday",
    .artist = "Traditional",
    .notes = happy_birthday_notes,
    .information = "Happy Birthday to You is a song traditionally sung to celebrate a person's birthday. The melody is from the song 'Good Morning to All', which was written by Patty Hill and her sister Mildred J. Hill in 1893.",
    .note_count = sizeof(happy_birthday_notes) / sizeof(Note)
};

Song super_mario_song = {
    .code = "super_mario",
    .title = "Super Mario Bros Theme (Intro)",
    .artist = "Koji Kondo",
    .notes = super_mario_notes,
    .information = "A short version of the Super Mario Bros. theme - originally composed by Koji Kondo in 1985 for Nintendo.",
    .note_count = sizeof(super_mario_notes) / sizeof(Note)
};

Song* songList = NULL;
size_t numOfSongs = 0;
bool songLibraryInitialized = false;

void init_song_library() {
    numOfSongs = 4;
    songList = (Song*)malloc(numOfSongs * sizeof(Song));
    if (songList == NULL) {
        printf("Failed to allocate memory for song library\n");
        return;
    }

    songList[0] = fur_elise_song;
    songList[1] = twinkle_twinkle_song;
    songList[2] = happy_birthday_song;
    songList[3] = super_mario_song;
}

void destroy_song_library() {
    if (songList != NULL) {
        free(songList);
        songList = NULL;
        songLibraryInitialized = false;
    }
}

void list_songs() {
    printf("\nAvailable songs:\n");
    for (size_t i = 0; i < numOfSongs; i++) {
        printf("%s - %s by %s\n", songList[i].code, songList[i].title, songList[i].artist);
    }
    printf("\n");
}

int get_song_index(char* song_code) {
    for (size_t i = 0; i < numOfSongs; i++) {
        if (strcmp(songList[i].code, song_code) == 0) {
            return (int)i;
        }
    }
    return -1;
}