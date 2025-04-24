#include "song.h"
#include "frequencies.h"   // if you choose to use named constants

// Define an array of notes:
Note music_1[] = {
    { C4, 500 },
    { D4, 500 },
    { E4, 500 },
    { C4, 500 },
    { D4, 500 },
    { E4, 500 },
    { C4, 500 },
    { D4, 500 },
    { E4, 500 },
    // …
};
// And its length:
size_t music_1_length = sizeof(music_1) / sizeof(music_1[0]);
