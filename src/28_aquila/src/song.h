#ifndef SONG_H
#define SONG_H

#include <libc/stddef.h> 
#include <libc/stdint.h> 

// Octave 4
#define NOTE_C4      262
#define NOTE_Csharp4 277 // C#4 / Db4
#define NOTE_D4      294
#define NOTE_Dsharp4 311 // D#4 / Eb4
#define NOTE_E4      330
#define NOTE_F4      349
#define NOTE_Fsharp4 370 // F#4 / Gb4
#define NOTE_G4      392
#define NOTE_Gsharp4 415 // G#4 / Ab4
#define NOTE_A4      440
#define NOTE_Asharp4 466 // A#4 / Bb4
#define NOTE_B4      494
// Octave 5
#define NOTE_C5      523
#define NOTE_Csharp5 554 // C#5 / Db5
#define NOTE_D5      587
#define NOTE_Dsharp5 622 // D#5 / Eb5
#define NOTE_E5      659
#define NOTE_F5      698
#define NOTE_Fsharp5 740 // F#5 / Gb5
#define NOTE_G5      784
#define NOTE_Gsharp5 831 // G#5 / Ab5
#define NOTE_A5      880
#define NOTE_Asharp5 932 // A#5 / Bb5
#define NOTE_B5      988
// Octave 6
#define NOTE_C6      1047
#define NOTE_Csharp6 1109 // C#6 / Db6
#define NOTE_D6      1175
#define NOTE_Dsharp6 1245 // D#6 / Eb6
#define NOTE_E6      1319
#define NOTE_F6      1397
#define NOTE_Fsharp6 1480 // F#6 / Gb6
#define NOTE_G6      1568
#define NOTE_Gsharp6 1661 // G#6 / Ab6
#define NOTE_A6      1760
#define NOTE_Asharp6 1865 // A#6 / Bb6
#define NOTE_B6      1976
// Octave 7
#define NOTE_C7      2093

#define NOTE_REST    0

typedef struct {
    uint32_t frequency; // hz
    uint32_t duration;  // ms
} Note;

typedef struct {
    Note* notes;  
    size_t note_count; 
} Song;

typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

// Converted from RTTTL: TopGun:d=4,o=4,b=31
// Quarter note duration approx 1935 ms
// 16th note duration approx 484 ms
// 32nd note duration approx 242 ms
static Note music_topgun[] = {
    {NOTE_REST, 242}, {NOTE_Csharp4, 484}, {NOTE_Gsharp4, 484}, {NOTE_Gsharp4, 484},
    {NOTE_Fsharp4, 242}, {NOTE_F4, 242}, {NOTE_Fsharp4, 242}, {NOTE_F4, 242},
    {NOTE_Dsharp4, 484}, {NOTE_Dsharp4, 484}, {NOTE_Csharp4, 242}, {NOTE_Dsharp4, 242},
    {NOTE_F4, 484}, {NOTE_Dsharp4, 242}, {NOTE_F4, 242}, {NOTE_Fsharp4, 484},
    {NOTE_F4, 242}, {NOTE_Csharp4, 242}, {NOTE_F4, 484}, {NOTE_Dsharp4, 1935},
    {NOTE_Csharp4, 484}, {NOTE_Gsharp4, 484}, {NOTE_Gsharp4, 484}, {NOTE_Fsharp4, 242},
    {NOTE_F4, 242}, {NOTE_Fsharp4, 242}, {NOTE_F4, 242}, {NOTE_Dsharp4, 484},
    {NOTE_Dsharp4, 484}, {NOTE_Csharp4, 242}, {NOTE_Dsharp4, 242}, {NOTE_F4, 484},
    {NOTE_Dsharp4, 242}, {NOTE_F4, 242}, {NOTE_Fsharp4, 484}, {NOTE_F4, 242},
    {NOTE_Csharp4, 242}, {NOTE_Gsharp4, 1935}
};

// Converted from RTTTL: Deep Purple-Smoke on the Water:o=4,d=4,b=112
// Quarter note duration = 536 ms
// Eighth note duration = 268 ms
static Note music_smoke[] = {
    {NOTE_C4, 536}, {NOTE_Dsharp4, 536}, {NOTE_F4, 804}, {NOTE_C4, 536},      // c,d#,f.,c
    {NOTE_Dsharp4, 536}, {NOTE_Fsharp4, 268}, {NOTE_F4, 536}, {NOTE_REST, 536}, // d#,8f#,f,p
    {NOTE_C4, 536}, {NOTE_Dsharp4, 536}, {NOTE_F4, 804}, {NOTE_Dsharp4, 536}, // c,d#,f.,d#
    {NOTE_C4, 536}, {NOTE_REST, 1072}, {NOTE_REST, 268}, /*{NOTE_C4, 536},      // c,2p,8p,c
    {NOTE_Dsharp4, 536}, {NOTE_F4, 804}, {NOTE_C4, 536}, {NOTE_Dsharp4, 536}, // d#,f.,c,d#
    {NOTE_Fsharp4, 268}, {NOTE_F4, 536}, {NOTE_REST, 536}, {NOTE_C4, 536},      // 8f#,f,p,c
    {NOTE_Dsharp4, 536}, {NOTE_F4, 804}, {NOTE_Dsharp4, 536}, {NOTE_C4, 536} */       // d#,f.,d#,c
};

// Converted from RTTTL: Rich Man's World:o=6,d=8,b=112
// Quarter note duration = 536 ms
// Eighth note duration = 268 ms
// 16th note duration = 134 ms
// 32nd note duration = 67 ms
static Note music_richmans[] = {
    {NOTE_E6, 268}, {NOTE_E6, 268}, {NOTE_E6, 268}, {NOTE_E6, 268}, {NOTE_E6, 268}, {NOTE_E6, 268}, // e,e,e,e,e,e
    {NOTE_E5, 134}, {NOTE_A5, 134}, {NOTE_C6, 134}, {NOTE_E6, 134}, // 16e5,16a5,16c,16e
    {NOTE_Dsharp6, 268}, {NOTE_Dsharp6, 268}, {NOTE_Dsharp6, 268}, {NOTE_Dsharp6, 268}, {NOTE_Dsharp6, 268}, {NOTE_Dsharp6, 268}, // d#,d#,d#,d#,d#,d#
    {NOTE_F5, 134}, {NOTE_A5, 134}, {NOTE_C6, 134}, {NOTE_Dsharp6, 134}, // 16f5,16a5,16c,16d#
    {NOTE_D6, 536}, {NOTE_C6, 268}, {NOTE_A5, 268}, {NOTE_C6, 268}, // 4d,c,a5,c
    {NOTE_C6, 536}, {NOTE_A5, 1072}, // 4c,2a5
    {NOTE_A5, 67}, {NOTE_C6, 67}, {NOTE_E6, 67}, // 32a5,32c,32e
    {NOTE_A6, 268} // a6
};

// Converted from RTTTL: Nokia:d=4,o=5,b=180
// Quarter note duration = 333 ms
// Eighth note duration = 167 ms
static Note music_nokia[] = {
    {NOTE_E6, 167}, {NOTE_D6, 167}, {NOTE_Fsharp5, 333}, {NOTE_Gsharp5, 333}, // 8e6,8d6,f#5,g#5
    {NOTE_Csharp6, 167}, {NOTE_B5, 167}, {NOTE_D5, 333}, {NOTE_E5, 333},      // 8c#6,8b5,d5,e5
    {NOTE_B5, 167}, {NOTE_A5, 167}, {NOTE_Csharp5, 333}, {NOTE_E5, 333},      // 8b5,8a5,c#5,e5
    {NOTE_A5, 333}, {NOTE_E5, 250}                                            // a5, e5, 
};

SongPlayer* create_song_player();

void play_music();

#endif // SONG_H
