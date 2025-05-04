#ifndef MEMORY_H
#define MEMORY_H

#include <libc/stdint.h>
#include "terminal.h"


typedef struct
{
    uint8_t status;
    uint32_t size;
} alloc_t;

void init_kernel_memory(uint32_t* kernel_end);


extern void init_paging();
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);
extern void paging_enable();

extern void* malloc(size_t size);
extern void free(void *mem);
//extern void* pmalloc(size_t size);
extern char* pmalloc(size_t size);


extern void *memset(void *ptr, int value, size_t num);
extern void *memset16(void *ptr, uint16_t value, size_t num);
extern void *memcpy(void *dest, const void *src, size_t count);


void print_memory_layout();

#endif