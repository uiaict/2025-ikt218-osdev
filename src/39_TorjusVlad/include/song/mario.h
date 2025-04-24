#ifndef MUSIC_MARIO_H
#define MUSIC_MARIO_H

#include "note.h"

Note music_mario[] = {
    {659, 125}, {659, 125}, {0, 125}, {659, 125},
    {0, 167}, {523, 125}, {659, 125}, {0, 125},
    {784, 125}, {0, 375}, {392, 125}, {0, 375}
};

size_t music_mario_len = sizeof(music_mario) / sizeof(Note);

#endif
