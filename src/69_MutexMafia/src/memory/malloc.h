#ifndef MALLOC_H
#define MALLOC_H
#include "libc/stdint.h"
#include "libc/stdbool.h"

typedef struct memoryBlock
{
    uint32_t size;
    bool isFree;
    struct memoryBlock* next;
    struct memoryBlock* prev;
} __attribute__((packed)) memoryBlock_t;

void* malloc(uint32_t size);
void init_kernel_memory(uint32_t* endAddr);
void free(void* ptr);
void print_memory_layout();

#endif
