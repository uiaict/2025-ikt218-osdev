#ifndef MEMORY_H
#define MEMORY_H


#include "libc/system.h"


typedef struct {
    uint8_t status;   // 0 = free, 1 = used
    uint32_t size;    // Size of the user data area
} alloc_t;



// Initializes the kernel's heap and pheap)
void init_kernel_memory(uint32_t* kernel_end_addr);


extern void init_paging();                                      // Enables paging
extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys); // Maps a virtual page to a physical frame


// Main heap allocation
extern void* malloc(size_t size);
extern void free(void *mem);


// Page-aligned heap allocation
extern char* pmalloc(size_t size);


extern void* memcpy(void* dest, const void* src, size_t num); // Copies 'num' bytes from 'src' to 'dest'
extern void* memset(void* ptr, int value, size_t num);       // Places 'num' bytes at 'ptr' with 'value'
extern void* memset16(void* ptr, uint16_t value, size_t num); // Places 'num' 16-bit words at 'ptr' with 'value'

void print_memory_layout(); // Prints statistics about current mem. usage and heap layout

#endif