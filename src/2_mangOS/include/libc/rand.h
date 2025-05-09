#pragma once
#ifndef UIAOS_RAND_H
#define UIAOS_RAND_H

#include "libc/stdint.h"

void srand(uint32_t seed);
int rand(void);

#endif // UIAOS_RAND_H
