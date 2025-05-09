#include "stdint.h"
#include "rng.h"
#include "pit.h"

static uint32_t rand_seed = 0xDEADBEEF;

uint32_t rng_get_seed() {
    return rand_seed;
}

void srand(uint32_t seed) {
    rand_seed = seed;
}

void srand_pit() {
    rand_seed = pit_get_ticks();
}

void rng_seed_xor(uint32_t n) {
    rand_seed ^= n;
}

uint32_t rand() {
    rand_seed ^= rand_seed << 13;
    rand_seed ^= rand_seed >> 17;
    rand_seed ^= rand_seed << 5;
    return rand_seed;
}

// unbiased as hek bro
uint32_t rand_range(uint32_t min, uint32_t max) {
    if (min == max) return min;
    if (min >= max) return min;

    uint32_t range = max - min + 1;
    uint32_t limit = 0xFFFFFFFF - (0xFFFFFFFF % range);

    uint32_t r;
    do {
        r = rand();
    } while (r >= limit);

    return min + (r % range);
}

// DnD is kewl
uint32_t roll_dice(uint32_t count, uint32_t sides) {
    uint32_t total = 0;
    for (uint32_t i = 0; i < count; i++) {
        total += rand_range(1, sides);
    }
    return total;
}