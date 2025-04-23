#include "libc/stdlib.h"

// Simple linear congruential generator
static unsigned int next = 1;

int rand() {
    next = next * 1103515245 + 12345;
    return (next / 65536) % 32768;
}

void srand(unsigned int seed) {
    next = seed;
}
