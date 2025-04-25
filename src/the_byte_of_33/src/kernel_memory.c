#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>


static uintptr_t kernel_heapS;
static uintptr_t kernel_heapE;
static uintptr_t kernel_heapC;

void init_kernel_memory(void* kernel_end) {
    kernel_heapS = (uintptr_t)kernel_end;
    kernel_heapE = kernel_heapS + 0x1000000;
    kernel_heapC = kernel_heapS;
}

void* malloc(size_t size) {
    if (kernel_heapC + size > kernel_heapE)
        return NULL;
    void* ptr = (void*)kernel_heapC;
    kernel_heapC += size;
    return ptr;
}

uintptr_t get_kernel_heap_start() {
    return kernel_heapS;
}

uintptr_t get_kernel_heap_end() {
    return kernel_heapE;
}