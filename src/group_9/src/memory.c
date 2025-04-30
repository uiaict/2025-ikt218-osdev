#include "memory.h"
#include "terminal.h"
#include <stdint.h>

extern uint32_t end;
static uint8_t* heap_start = NULL;
static uint8_t* heap_end = (uint8_t*)0x3E0000;
static uint8_t* heap_current = NULL;
static uint32_t* placement_address = 0;

// Initialize the kernel memory manager
void init_kernel_memory(uint32_t* kernel_end) {
    placement_address = kernel_end;
    heap_start = (uint8_t*)kernel_end;   // heap_start starting here
    heap_current = (uint8_t*)kernel_end; // heap_current is initially the same as heap_start
}

// Very simple malloc function (bump allocator)
void* malloc(uint32_t size) {
    void* addr = placement_address;
    placement_address = (uint32_t*)((uint32_t)placement_address + size);
    heap_current += size;  // When malloc is done, heap_current is advanced
    return addr;
}

// Free (no-op for now)
void free(void* ptr) {
    // Şu anda free işlemi yapmıyoruz (bump allocator)
}

// This function shows heap start and usage status
void print_memory_layout() {
    uint32_t heap_size = (uint32_t)(heap_end - heap_start);
    uint32_t memory_used = (uint32_t)(heap_current - heap_start);
    uint32_t memory_free = (uint32_t)(heap_end - heap_current);

    terminal_printf("[INFO] Heap layout information:\n");
    terminal_printf("Heap start address: 0x%x\n", (uint32_t)heap_start);
    terminal_printf("Heap end address:   0x%x\n", (uint32_t)heap_end);
    terminal_printf("Heap total size:    %d bytes\n", heap_size);
    terminal_printf("Memory used:        %d bytes\n", memory_used);
    terminal_printf("Memory free:        %d bytes\n\n", memory_free);
}
