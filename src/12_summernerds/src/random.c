#include "random.h"
int next = -1;

// starts
void setupRNG(int seed)
{
    next = seed;
}
// Generates random number. setupRNG needs to be run first
int randint(int max)
{
    if (next < 0)
        return -1;
    next = next * 1584506493 + 69420;
    return (unsigned)(next / 24947) % max;
}