#ifndef MALLOC_H
#define MALLOC_H

#include "libc/system.h"
#include "printf.h"
#include "utils.h"

#define MAX_HEAP_SIZE (64 * 1024 * 1024) // 64MB

typedef struct
{
    uint32_t size;
    uint8_t status;
}   alloc_t;

void init_kernel_memory(uint32_t *endAddr);
void *malloc(size_t size);
void free(void *mem);
void print_memory_layout();

#endif