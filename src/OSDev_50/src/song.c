#include "song.h"
#include "frequencies.h"   // if you choose to use named constants

// Base durations at 140 BPM (ms)
#define HALF            857
#define DOTTED_QUARTER   643
#define QUARTER          429
#define EIGHTH           214

// Staccato: 80% sound, 20% rest
#define HALF_SOUND   686   // 857 * 0.8
#define HALF_REST    171   // 857 * 0.2
#define DQ_SOUND     514   // 643 * 0.8
#define DQ_REST      129   // 643 * 0.2
#define Q_SOUND      343   // 429 * 0.8
#define Q_REST        86   // 429 * 0.2
#define E_SOUND      171   // 214 * 0.8
#define E_REST        43   // 214 * 0.2

// Pitches (Hz)
#define B4   494
#define DS5  622
#define A4   440
#define G4   392
#define E4   330
#define REST   0

Note music_1[] = {
    // Phrase 1 (×3)
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },

    // Phrase 2
    { DS5, Q_SOUND },  { REST, Q_REST },
    { B4,  Q_SOUND },  { REST, Q_REST },
    { B4,  E_SOUND },  { REST, E_REST },
    { A4,  E_SOUND },  { REST, E_REST },
    { G4,  Q_SOUND },  { REST, Q_REST },
    { A4,  Q_SOUND },  { REST, Q_REST },
    { B4,  HALF_SOUND },{ REST, HALF_REST },

    // Phrase 3 = Phrase 1
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },
    { B4, DQ_SOUND }, { REST, DQ_REST },
    { B4, E_SOUND },   { REST, E_REST },

    // Phrase 4
    { B4,  Q_SOUND },  { REST, Q_REST },
    { A4,  E_SOUND },  { REST, E_REST },
    { G4,  E_SOUND },  { REST, E_REST },
    { A4,  Q_SOUND },  { REST, Q_REST },
    { G4,  DQ_SOUND }, { REST, DQ_REST },
    { E4,  E_SOUND },  { REST, E_REST },
    { E4,  HALF_SOUND },{ REST, HALF_REST },
};
// And its length:
size_t music_1_length = sizeof(music_1) / sizeof(music_1[0]);
