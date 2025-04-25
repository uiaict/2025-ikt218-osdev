#include "memory.h"
#include "io.h"

// Simple memory block structure
typedef struct mem_block {
    size_t size;        // Size of the block
    struct mem_block* next; // Pointer to the next block
    int free;               // 1 if free, 0 if allocated
} mem_block_t;