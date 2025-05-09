#include "song/song.h"
#include "song/frequencies.h"

Note music_1[] = {
    {E5, 250},
    {R, 125},
    {E5, 125},
    {R, 125},
    {E5, 125},
    {R, 125},
    {C5, 125},
    {E5, 125},
    {G5, 125},
    {R, 125},
    {G4, 125},
    {R, 250},
};
uint32_t music_1_len = sizeof(music_1) / sizeof(Note);

Note music_2[] = {
    {A4, 200},
    {E5, 200},
    {A5, 200},
    {R, 100},
};
uint32_t music_2_len = sizeof(music_2) / sizeof(Note);

Note music_3[] = {
    {E4, 200},
    {F4, 200},
    {G4, 200},
};
uint32_t music_3_len = sizeof(music_3) / sizeof(Note);

Note music_4[] = {
    {C4, 500},
    {D4, 500},
    {E4, 500},
    {C4, 500},
};
uint32_t music_4_len = sizeof(music_4) / sizeof(Note);

Note music_5[] = {
    {E4, 375},
    {C4, 375},
    {D4, 375},
    {A3, 375},
};
uint32_t music_5_len = sizeof(music_5) / sizeof(Note);

Note music_6[] = {
    {F4, 250},
    {F4, 250},
    {F4, 250},
};
uint32_t music_6_len = sizeof(music_6) / sizeof(Note);

Song song1 = {music_1, sizeof(music_1) / sizeof(Note)};
Song song2 = {music_2, sizeof(music_2) / sizeof(Note)};
Song song3 = {music_3, sizeof(music_3) / sizeof(Note)};
Song song4 = {music_4, sizeof(music_4) / sizeof(Note)};
Song song5 = {music_5, sizeof(music_5) / sizeof(Note)};
Song song6 = {music_6, sizeof(music_6) / sizeof(Note)};
