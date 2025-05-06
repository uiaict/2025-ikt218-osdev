#include "libc/stdio.h"
#include "libc/stdint.h"

extern uint32_t end;

void init_kernel_memory(uint32_t* kernel_end) {
    printf("Initializing kernel memory from: 0x%x\n", (uint32_t)kernel_end);
}

void print_memory_layout() {
    printf("Kernel memory layout:\n");
    printf(" - Kernel end: 0x%x\n", (uint32_t)&end);
    printf(" - Heap start: 0x100000\n");
}
