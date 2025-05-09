#include "random.h"

static unsigned long int rand_state;
extern volatile uint32_t tick;

int rand(void) {
    rand_state = rand_state * 1103515245 + 12345;
    return (unsigned int)(rand_state / 65536) % 32768;
}

int getRandomNumber(int min, int max){
    if (max <= min) return min;
    int r = rand();
    return min + (r % (max - min + 1));
}