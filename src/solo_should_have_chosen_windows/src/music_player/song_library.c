#include "music_player/song_library.h"
#include "music_player/frequencies.h"

static Note fur_elise_notes[] = {
    {E5, 150},
    {0, 20},   // Pause
    {Ds5, 150},
    {0, 20},   // Pause
    {E5, 150},
    {0, 20},   // Pause
    {Ds5, 150},
    {0, 20},   // Pause
    {E5, 150},
    {0, 20},   // Pause
    {B4, 150},
    {0, 20},   // Pause
    {D5, 150},
    {0, 20},   // Pause
    {C5, 150},
    {0, 20},   // Pause
    {A4, 150},
    {0, 20},   // Pause
    {C4, 150},
    {0, 20},   // Pause
    {E4, 150},
    {0, 20},   // Pause
    {A4, 150},
    {0, 20},   // Pause
    {B4, 200},
    {0, 20},   // Pause
    {E4, 150},
    {0, 20},   // Pause
    {A4, 150},
    {0, 20},   // Pause
    {B4, 150},
    {0, 20},   // Pause
    {C5, 500},
    {0, 20}    // Pause
};

Song fur_elise_song = {
    .notes = fur_elise_notes,
    .note_count = sizeof(fur_elise_notes) / sizeof(Note)
};