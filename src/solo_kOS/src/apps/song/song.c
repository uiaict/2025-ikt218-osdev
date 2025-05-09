#include "apps/song/song.h"

// === Song Definitions ===

static Note food_ding[] = {
    {A5, 20},
};

static Note confirm_beep[] = {
    {C6, 20},
};
static Note fail_beep[] = {
    {C3, 20},
};
static Note pause_beep[] = {
    {C5, 20},
};

Song song_food = {
    food_ding,
    sizeof(food_ding) / sizeof(Note)
};

Song song_confirm = {
    confirm_beep,
    sizeof(confirm_beep) / sizeof(Note)
};

Song song_fail = {
    fail_beep,
    sizeof(fail_beep) / sizeof(Note)
};

Song song_pause = {
    pause_beep,
    sizeof(pause_beep) / sizeof(Note)
};

static Note music_5[] = {
    {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375},
    {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375},
};

Song song_open = {
    music_5,
    sizeof(music_5) / sizeof(Note)
};

static Note music_1[] = {
    {E5, 250}, {R, 125}, {E5, 125}, {R, 125}, {E5, 125}, {R, 125},
    {C5, 125}, {E5, 125}, {G5, 125}, {R, 125}, {G4, 125}, {R, 250},

    {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125},
    {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125},
    {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125},
    {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125},

    {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125},
    {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125},
    {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125},
    {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125},
};

static Note music_2[] = {
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}
};

static Note music_3[] = {
    {E4, 200}, {E4, 200}, {F4, 200}, {G4, 200}, {G4, 200}, {F4, 200}, {E4, 200}, {D4, 200},
    {C4, 200}, {C4, 200}, {D4, 200}, {E4, 200}, {E4, 400}, {R, 200},
    {D4, 200}, {D4, 200}, {E4, 200}, {F4, 200}, {F4, 200}, {E4, 200}, {D4, 200}, {C4, 200},
    {A4, 200}, {A4, 200}, {A4, 200}, {G4, 400}
};

static Note music_4[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
    {C4, 500}, {G3, 500}, {C4, 1000},
    {C4, 500}, {G3, 500}, {C4, 1000}
};



static Note music_6[] = {
    {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500},
    {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500},
    {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500},
    {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500},
};

static Note starwars_theme[] = {
    // Opening phrase
    {A4, 500}, {A4, 500}, {A4, 500}, 
    {F4, 375}, {C5, 125}, 
    {A4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, 
    {E5, 500}, {E5, 500}, {E5, 500}, 
    {F5, 375}, {C5, 125},
    
    // Next phrase
    {G4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, 
    {A5, 500}, {A4, 375}, {A4, 125}, 
    {A5, 500}, {G5, 375}, {F5, 125}, {E5, 125}, {D5, 125}, 
    {C5, 250}, {B4, 250}, {A4, 500},

    // End note
    {R, 500}
};

Song song_music_1 = { music_1, sizeof(music_1) / sizeof(Note) };
Song song_music_2 = { music_2, sizeof(music_2) / sizeof(Note) };
Song song_music_3 = { music_3, sizeof(music_3) / sizeof(Note) };
Song song_music_4 = { music_4, sizeof(music_4) / sizeof(Note) };
Song song_music_5 = { music_5, sizeof(music_5) / sizeof(Note) }; 
Song song_music_6 = { music_6, sizeof(music_6) / sizeof(Note) };
Song song_starwars = { starwars_theme, sizeof(starwars_theme) / sizeof(Note) };