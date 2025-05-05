#include <utils.h>

// rand() function implementation
static unsigned long next = 1;
int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
int srand(unsigned int seed) {
    next = seed;
    return next;
}