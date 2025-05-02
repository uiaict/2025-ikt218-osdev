#include "kernel/malloc.h"
#include "libc/stdbool.h"

// #define MEMORY_CHUNKS 512

// This seems to be the first safe memory address available for
// allocation on X86.
#define FIRST_MEMORY_ADDRESS 0x000FFFFF
#define LAST_MEMORY_ADDRESS 0x7FFFFFFF

int last_allocated_memory_address = FIRST_MEMORY_ADDRESS;

void *malloc(int bytes) {
  if (last_allocated_memory_address + bytes > LAST_MEMORY_ADDRESS) {
    return 0; // Out of memory
  } else {
    void *next_memory_address = (void *)last_allocated_memory_address;
    last_allocated_memory_address += bytes;
    return (void *)next_memory_address;
  }
}

void free(void *pointer) {}