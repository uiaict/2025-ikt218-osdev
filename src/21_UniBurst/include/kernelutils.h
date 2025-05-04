#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include "libc/stdint.h"

void panic(const char *msg);
void debug_print_address(uint32_t address);

#endif // KERNEL_UTILS_H