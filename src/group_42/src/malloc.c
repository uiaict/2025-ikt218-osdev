#include "malloc.h"
#include "../include/libc/stdbool.h"

// #define MEMORY_CHUNKS 512

// This seems to be the first safe memory address available for
// allocation on X86.
#define FIRST_MEMORY_ADDRESS 0x000FFFFF
#define LAST_MEMORY_ADDRESS 0x7FFFFFFF

int last_allocated_memory_address = FIRST_MEMORY_ADDRESS;

void *malloc(int bytes) {
  last_allocated_memory_address += bytes;
  if (last_allocated_memory_address > LAST_MEMORY_ADDRESS) {
    return 0; // Out of memory
  } else {
    return (void *)last_allocated_memory_address;
  }
}

void free(void *pointer) {}