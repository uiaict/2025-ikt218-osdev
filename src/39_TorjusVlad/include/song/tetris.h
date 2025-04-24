#ifndef MUSIC_TETRIS_H
#define MUSIC_TETRIS_H

#include "note.h"

Note music_tetris[] = {
    {440, 150}, {659, 150}, {587, 150}, {523, 150},
    {494, 150}, {523, 150}, {587, 150}, {659, 150},
    {587, 150}, {523, 150}, {494, 150}, {440, 150}
};

size_t music_tetris_len = sizeof(music_tetris) / sizeof(Note);

#endif