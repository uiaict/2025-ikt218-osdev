#ifndef MEMORY_H
#define MEMORY_H

#include <libc/stddef.h>
#include <libc/stdint.h>

void init_kernel_memory(uint32_t *kernel_end);
void *malloc(size_t size);
void free(void *ptr);
void print_memory_layout();

#endif
