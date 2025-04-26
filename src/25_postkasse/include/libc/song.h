// filepath: /workspaces/2025-ikt218-osdev/src/25_postkasse/include/libc/song.h
#ifndef SONG_H
#define SONG_H

#include "libc/system.h"
#include "libc/frequencies.h"

//Structure of a note
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

extern Note happy_birthday[];
extern Note star_wars_theme[];
extern Note fur_elise[];


extern const size_t HAPPY_BIRTHDAY_LENGTH;
extern const size_t STAR_WARS_THEME_LENGTH;
extern const size_t FUR_ELISE_LENGTH;







#endif