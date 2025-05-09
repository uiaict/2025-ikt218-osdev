#include "song.h"

// Harry Potter theme simplified melody
Note harry_potter_melody[] = {
    { 523, 500 },  // C5
    { 523, 500 },  // C5
    { 659, 500 },  // E5
    { 523, 500 },  // C5
    { 784, 500 },  // G5
    { 698, 500 },  // F5
    { 523, 500 },  // C5
    { 523, 500 },  // C5
    { 659, 500 },  // E5
    { 523, 500 },  // C5
    { 784, 500 },  // G5
    { 698, 500 },  // F5
    { 523, 500 },  // C5

    // Repeating pattern
    { 523, 500 },  // C5
    { 523, 500 },  // C5
    { 659, 500 },  // E5
    { 523, 500 },  // C5
    { 784, 500 },  // G5
    { 698, 500 },  // F5
    { 523, 500 },  // C5
    { 523, 500 },  // C5
    { 659, 500 },  // E5
    { 523, 500 },  // C5
    { 784, 500 },  // G5
    { 698, 500 },  // F5
    { 523, 500 },  // C5
};

// Define the song structure
Song harry_potter_song = {
    .notes = harry_potter_melody,
    .length = sizeof(harry_potter_melody) / sizeof(Note)
};
