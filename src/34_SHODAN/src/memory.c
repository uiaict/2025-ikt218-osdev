#include "stdint.h"
#include "terminal.h"

#define HEAP_LIMIT 0x1000000
#define ALIGN4(x) (((x) + 3) & ~3)

static uint8_t* heap_start;
static uint8_t* heap_end;

void init_kernel_memory(uint32_t* kernel_end) {
    heap_start = (uint8_t*)ALIGN4((uintptr_t)kernel_end); // uintptr_t now defined
    heap_end = heap_start;
}

void* malloc(uint32_t size) {
    if ((uintptr_t)(heap_end + size) >= HEAP_LIMIT) {
        terminal_write("Out of heap memory!\n");
        return 0;
    }

    void* ptr = heap_end;
    heap_end += size;
    return ptr;
}

void free(void* ptr) {
    // Not implemented
}

void print_memory_layout() {
    terminal_write("Memory Layout:\n");
    terminal_write("  Heap Start: ");
    terminal_putint((uint32_t)heap_start);
    terminal_write("\n  Heap End:   ");
    terminal_putint((uint32_t)heap_end);
    terminal_write("\n");
}
