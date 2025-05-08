#include "libc/random.h"
#include "pit.h"

static size_t seed = 0;

void random_seed(size_t s) {
    seed = s;
}

float random() {
    size_t clock = get_current_tick();
    float rand = 0;

    if (seed == 0) {
        rand = (1103515245 * clock + 12345) & 0x7fffffff;    
    } else {
        rand = (1103515245 * (clock + seed) + 12345) & 0x7fffffff; 
    }

    return rand / 2147483647.0f; // devidin to get number in range from 0 to 1    
}