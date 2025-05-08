#ifndef TIMER_H
#define TIMER_H

#include "common.h"

// Initialize timer with given frequency
void init_timer(u32int frequency);

// Sleep for specified milliseconds
void sleep(u32int ms);

#endif
