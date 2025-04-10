#include "memory.h"
#include "terminal.h"

#define HEAP_START 0x100000  // 1 MB
#define HEAP_SIZE  0x300000  // 3 MB (total heap memory)
#define MAX_BLOCKS 1024      // Maximum number of tracked allocations

// External variables from memory.c
extern void* heap_base;
extern uint32_t heap_used;
extern alloc_t alloc_table[1024];

void* malloc(size_t size) {
    if (size == 0 || heap_used + size > HEAP_SIZE) {
        terminal_write("malloc failed: not enough heap space\n");
        return NULL;
    }

    // Find a free slot in the alloc_table
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (alloc_table[i].status == 0) {
            void* ptr = (uint8_t*)heap_base + heap_used;
            alloc_table[i].status = 1;
            alloc_table[i].size = size;
            heap_used += size;
            return ptr;
        }
    }

    terminal_write("malloc failed: alloc_table full\n");
    return NULL;
}

void free(void* ptr) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (alloc_table[i].status == 1 &&
            ((uint8_t*)ptr >= (uint8_t*)heap_base) &&
            ((uint8_t*)ptr < (uint8_t*)heap_base + HEAP_SIZE)) {
            alloc_table[i].status = 0;
            return;
        }
    }
}
