#ifndef MUSIC_ZELDA_H
#define MUSIC_ZELDA_H

#include "note.h"

Note music_zelda[] = {
    {659, 200}, {784, 200}, {880, 200}, {698, 200}, {784, 400}, {659, 200}
};

size_t music_zelda_len = sizeof(music_zelda) / sizeof(Note);

#endif