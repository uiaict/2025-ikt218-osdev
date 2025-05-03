#include "memory/memory.h"
#include "libc/stdio.h"

#define KERNEL_HEAP_SIZE 0x100000  // 1MB heap
static uint8_t* heap_start;
static uint8_t* heap_end;
static uint8_t* heap_curr;

void init_kernel_memory(uint32_t* kernel_end) {
    heap_start = (uint8_t*)((uint32_t)kernel_end);
    heap_end = heap_start + KERNEL_HEAP_SIZE;
    heap_curr = heap_start;
    printf("[MEMORY] Kernel heap initialized at: 0x%x\n", (uint32_t)heap_start);
}

void* malloc(size_t size) {
    if (heap_curr + size > heap_end) return NULL;

    void* addr = (void*)heap_curr;
    heap_curr += size;
    return addr;
}

void free(void* ptr) {
    // No-op: simple bump allocator (no free)
}
