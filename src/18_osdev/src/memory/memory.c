#include "memory.h"
#include "libc/stdint.h"
#include "libc/monitor.h"

// stores the top of the heap
static uint8_t* heap_ptr = 0;
static uint8_t* heap_start = 0;
extern uint32_t end;

// sets starting point of heap
void init_kernel_memory(uint32_t* kernel_end) {
    // initialize the heap pointer to end of the kernel
    heap_ptr = (uint8_t*)kernel_end;
    // saving the startpoint of the heap
    heap_start = heap_ptr;
}

// bump implementation of malloc
void* malloc(uint32_t size) {
    void* block = (void*)heap_ptr; // sets starting point of the allocated memory
    heap_ptr += size;             // sets endpoint of the allocated memory
    return block;
}

uint32_t get_heap_used() {
    return (uint32_t)(heap_ptr - heap_start);
}

void free(void* ptr) {
    // no need for free when using bump allocator
}

void print_memory_layout() {
    monitor_write("Kernel end: ");
    monitor_write_hex((uint32_t)&end);
    monitor_write("\n");

    monitor_write("Heap start: ");
    monitor_write_hex((uint32_t)heap_start);
    monitor_write("\n");

    // Optional: show how much memory used
    monitor_write("Heap used: ");
    monitor_write_dec((uint32_t)(heap_ptr - (uint8_t*)&end));
    monitor_write(" bytes\n");
}