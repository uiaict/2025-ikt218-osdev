// memory.h

#ifndef __MEMORY_H_
#define __MEMORY_H_

// Include standard types
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/common.h" // Include common.h to avoid conflicts

typedef struct 
{
    uint8_t status;
    uint32_t size;
} alloc_t;

extern void init_kernel_memory(uint32_t kernel_end);
extern void print_memory_layout();

extern void paging_init();
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

extern char* pmalloc(size_t size);
extern char* malloc(size_t size);
extern void free(void *mem);

extern void* memset16(void *ptr, uint16_t value, size_t num);

#endif