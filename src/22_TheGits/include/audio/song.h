#ifndef SONG_H
#define SONG_H

#include "libc/stdint.h"
#include "audio/frequencies.h"

// Define a struct to represent a single musical note
typedef struct {
    uint32_t frequency;
    uint32_t duration;  
} Note;

// Define a struct to represent a song, which is an array of notes
typedef struct {
    Note* notes;        
    uint32_t length;    
} Song;


#endif
