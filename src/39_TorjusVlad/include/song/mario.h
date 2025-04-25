#ifndef MUSIC_MARIO_H
#define MUSIC_MARIO_H

#include "note.h"

Note music_mario[] = {
    {659, 125}, {659, 125}, {0, 125}, {659, 125},
    {0, 167}, {523, 125}, {659, 125}, {0, 125},
    {784, 125}, {0, 375}, {392, 125}, {0, 375}
};

Note mario_new[] =  {  {330, 100}, {330, 100}, {330, 100}, {262, 100},  // G4, G4, pause, G4 (First line)
{330, 100}, {392, 100}, {196, 100}, {262, 300},  // pause, G4, pause, G4
{196, 300}, {164, 300}, {196, 300}, {164, 300},  // E5, E5, E5, pause
{220, 300}, {246, 100}, {233, 200}, {0, 125},  // G4, G4, G4, pause
 {523, 250}, {0, 125}, {523, 250}, {0, 250},  // G4, pause, G4, pause
 {523, 250}, {523, 250}, {659, 250}, {659, 500},  // G4, G4, E5, E5
 {0, 125}, {523, 250}, {0, 250}, {523, 250},  // pause, G4, pause, G4
 {659, 250}, {659, 250}, {659, 500}, {0, 125},  // E5, E5, E5, pause
 {523, 250}, {523, 250}, {523, 250}, {0, 125},  // G4, G4, G4, pause
 {523, 250}, {0, 125}, {523, 250}, {0, 250},  // G4, pause, G4, pause
 {0, 500} }; // Pause at the end to stop

size_t music_mario_len = sizeof(music_mario) / sizeof(Note);
size_t mario_new_len = sizeof(mario_new) / sizeof(Note);

#endif
