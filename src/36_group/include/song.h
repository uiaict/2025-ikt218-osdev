#ifndef SONG_H
#define SONG_H

#include <libc/stdint.h>
#include <libc/stdbool.h>
#include "frequencies.h"

typedef enum
{
    SONG_COMPLETED,
    SONG_INTERRUPTED_NEXT,
    SONG_INTERRUPTED_BACK,
    SONG_INTERRUPTED_SELECT,
    SONG_INTERRUPTED_PREV
} SongResult;

typedef struct
{
    uint32_t frequency;
    uint32_t duration;
} Note;

typedef struct Song
{
    Note *notes;
    uint32_t length;
} Song;

typedef struct SongPlayer
{
    SongResult (*play_song)(struct SongPlayer *player, struct Song *song);
    bool is_playing;
} SongPlayer;

SongPlayer *create_song_player();
void free_song_player(SongPlayer *player);
SongResult play_song(struct SongPlayer *player, struct Song *song);

// === SONG DATA ===

static Note music_1[] = {
    {E5, 250}, {R, 125}, {E5, 125}, {R, 125}, {E5, 125}, {R, 125}, {C5, 125}, {E5, 125}, {G5, 125}, {R, 125}, {G4, 125}, {R, 250}, {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125}, {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125}, {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125}, {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125}, {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125}, {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125}, {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125}, {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125}};

static Note starwars_theme[] = {
    {A4, 500}, {A4, 500}, {A4, 500}, {F4, 375}, {C5, 125}, {A4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, {E5, 500}, {E5, 500}, {E5, 500}, {F5, 375}, {C5, 125}, {G4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, {A5, 500}, {A4, 375}, {A4, 125}, {A5, 500}, {G5, 375}, {F5, 125}, {E5, 125}, {D5, 125}, {C5, 250}, {B4, 250}, {A4, 500}, {R, 500}};

static Note battlefield_1942_theme[] = {
    {E4, 500}, {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, {G5, 200}, {E5, 300}, {D5, 200}, {B4, 300}, {G4, 500}, {E4, 500}, {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, {G5, 200}, {E5, 300}, {D5, 200}, {B4, 300}, {G4, 500}, {R, 500}};

static Note music_2[] = {
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}};

static Note music_3[] = {
    {E4, 200}, {E4, 200}, {F4, 200}, {G4, 200}, {G4, 200}, {F4, 200}, {E4, 200}, {D4, 200}, {C4, 200}, {C4, 200}, {D4, 200}, {E4, 200}, {E4, 400}, {R, 200}, {D4, 200}, {D4, 200}, {E4, 200}, {F4, 200}, {F4, 200}, {E4, 200}, {D4, 200}, {C4, 200}, {A4, 200}, {A4, 200}, {A4, 200}, {G4, 400}};

static Note music_4[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500}, {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500}, {E4, 500}, {F4, 500}, {G4, 1000}, {E4, 500}, {F4, 500}, {G4, 1000}, {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500}, {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500}, {C4, 500}, {G3, 500}, {C4, 1000}, {C4, 500}, {G3, 500}, {C4, 1000}};

static Note music_5[] = {
    {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375}, {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375}};

static Note music_6[] = {
    {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500}, {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500}, {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500}, {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500}};

#endif
