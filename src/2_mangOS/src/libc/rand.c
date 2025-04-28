#include "libc/rand.h"
#include "libc/stdint.h"
static uint32_t seed = 1; // Initial seed

void srand(uint32_t s)
{
    seed = s;
}

int rand(void)
{
    // Simple linear congruential generator (LCG)
    seed = seed * 1664525 + 1013904223;
    return (seed >> 16) & 0x7FFF; // Return 15-bit random number
}
