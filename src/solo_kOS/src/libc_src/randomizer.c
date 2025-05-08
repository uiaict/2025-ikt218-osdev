#include "kernel/pit.h"
#include "libc/randomizer.h"

static uint32_t rng_state = 1; // 32-bit internal state (never zero)

// xorshift32 — 1 → 2^-32 PRNG step, cheap and decent for game
static inline uint32_t xorshift32(uint32_t x)
{
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

// Seed generator initlialized right after the pit at boot using PIT tick counter 
void rand_init(void)
{
    rng_state = pit_ticks() ^ 0xA5A5A5A5u;

    if (rng_state == 0)
        rng_state = 1;
}

// Return a 31-bit unsigned random value (0…2^31 - 1) 
uint32_t rand_u32(void)
{
    rng_state = xorshift32(rng_state);
    return rng_state >> 1;                
}

// Return a random value in the range [0, max) using modulo
uint32_t rand_range(uint32_t max)
{
    return rand_u32() % max;           
}

uint32_t rand_range_skip(uint32_t max,
    const uint32_t *exclude, size_t exclude_cnt)
    {
    while (1)
    {
        uint32_t v = rand_range(max);        // candidate

    // linear scan should be fine for the small lists a snake game has  
    bool clash = false;
    for (size_t i = 0; i < exclude_cnt; ++i)
    if (exclude[i] == v) { clash = true; break; }

    if (!clash)
        return v;                        // found a free slot
    }
}