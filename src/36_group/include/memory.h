#ifndef MEMORY_H
#define MEMORY_H

#include <libc/stdint.h>
#include <libc/stddef.h>

typedef struct mem_block
{
    size_t size;
    struct mem_block *next;
    int free;
} mem_block_t;

void init_kernel_memory(void *kernel_end);
void init_paging(void);
void print_memory_layout(void);
void *malloc(size_t size);
void free(void *ptr);

#endif