#include "memory_layout.h"
#include "kernel_memory.h"
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
#include <libc/stdbool.h>


extern uint32_t start;
extern uint32_t end;

void print_memory_layout() {
    printf("============= Memory Layout =============", 0);
    printf("Kernel Start Adress : 0x%08X\n", (uint32_t)&start);
    printf("Kernel End Address   : 0x%08X\n", (uint32_t)&end);
    printf("Kernel Heap Start Address : 0x%08X\n", get_kernel_heap_start());
    printf("Kernel Heap End Address   : 0x%08X\n", get_kernel_heap_end());
    printf("=========================================\n", 0);
}