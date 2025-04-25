#include <stddef.h>          // 🔧 nødvendig for size_t
#include "song/note.h"       // note-strukturen

Note music_1[] = {
    {440, 300},
    {494, 300},
    {523, 300},
    {0, 100},
    {523, 300}
};

const size_t music_1_len = sizeof(music_1) / sizeof(Note); // ✅
