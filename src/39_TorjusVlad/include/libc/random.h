#ifndef RANDOM_H
#define RANDOM_H

#include "libc/stdint.h"
#include "libc/stddef.h"

void random_seed(size_t s);
float random();

#endif // RANDOM_H