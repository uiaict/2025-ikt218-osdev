#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"

void init_kernel_memory(uint32_t* kernel_end);
void* malloc(uint32_t size);
uint32_t get_heap_used();
void free(void* ptr);
void print_memory_layout();

#endif