#pragma once

#include <libc/stdint.h> 
#include <libc/stddef.h> 

#define MAX_PAGE_ALIGNED_ALLOCS 32

// Implicit List // Best Fit
typedef struct alloc_t {
    size_t size;
    size_t free; // 0 = free, 1 = used
    struct alloc_t *next;
} alloc_t;

void init_kernel_memory(uint32_t *kernel_end);
void *malloc(size_t size);
void free(void *mem);
void *pmalloc();
void pfree(void *mem);
void print_memory_layout();

void init_paging();
extern void load_page_dir(unsigned int*);
extern void enable_paging();

