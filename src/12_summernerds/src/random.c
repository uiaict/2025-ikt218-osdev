#include "random.h"
static int next = 55;

// starts
void setupRNG(int seed)
{
    next = seed;
}
// Generates random number. setupRNG needs to be run first
int randint(int max)
{
    next = next * 158450649 + 69420;
    return (unsigned)(next / 24947) % max;
}