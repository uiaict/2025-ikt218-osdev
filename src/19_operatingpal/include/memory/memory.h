#pragma once
#include "libc/stdint.h"
#include "libc/stddef.h"

void init_kernel_memory(uint32_t* kernel_end);
void* malloc(size_t size);
void free(void* ptr);
