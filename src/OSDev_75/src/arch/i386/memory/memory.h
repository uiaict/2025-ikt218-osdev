#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

/* Init Kernel Memory */
void init_kernel_memory(uint32_t* kernel_end);

/* Function declarations for paging operations */
void init_paging();
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

/* Function declarations for memory allocation */
char* pmalloc(size_t size);
void* malloc(size_t size);
void free(void *mem);
void pfree(void *mem);

/* Function declarations for memory manipulation */
void* memcpy(void* dest, const void* src, size_t num);
void* memset(void* ptr, int value, size_t num);
void* memset16(void *ptr, uint16_t value, size_t num);

/* Other helper functions*/
void print_memory_layout();

#ifdef __cplusplus
}
#endif

#endif