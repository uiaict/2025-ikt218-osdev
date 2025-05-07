#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"

typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

void kernel_memory_init(uint32_t*);

void* malloc(size_t size);
void free(void* ptr);

#endif