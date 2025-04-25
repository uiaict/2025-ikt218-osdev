#ifndef SONG_H
#define SONG_H

#include <libc/stddef.h>
#include <libc/stdint.h>
#include "frequencies.h"

#define PC_SPEAKER_PORT 0x61

typedef struct {
    uint32_t frequency; 
    uint32_t duration;  
} Note;

typedef struct {
    Note* notes;
    size_t note_count;
} Song;

typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t frequency);
void stop_sound();
void play_song(Song* song);
void play_song_impl(Song* song);
SongPlayer* create_song_player();

static Note music_1[] = {
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

#endif /* SONG_H */