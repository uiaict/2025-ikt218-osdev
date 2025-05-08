#pragma once

#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdint.h"
#include "common/itoa.h"

int printf(const char* __restrict__ format, ...);

// terminal_write now just forwards to the monitor
void terminal_write(const char *str);

// Debugging helper to check segment registers
void check_segment_registers();
