#ifndef UTIL_H
#define UTIL_H

#include "stdint.h"
#include "types.h"

/**
 * Write an 8-bit value to the specified port.
 */
void outPortB(uint16_t port, uint8_t value);

/**
 * Read an 8-bit value from the specified port.
 */
char inPortB(uint16_t port);

#define CEIL_DIV(a,b) (((a + b) - 1)/(b))

#endif
