#ifndef KERNEL_STUFF_H
#define KERNEL_STUFF_H

#include "libc/stdint.h"

void panic(const char *msg);
void debug_print_address(uint32_t address);

#endif 