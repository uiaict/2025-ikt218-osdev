#pragma once

#include "libc/stddef.h"
#include "libc/stdint.h"


size_t strlen(const char* str);

void *memset(void *dest, int val, size_t len);