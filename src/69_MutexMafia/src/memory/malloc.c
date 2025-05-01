#include "malloc.h"
#include "libc/stdint.h"
#include "libc/string.h"
#include "libc/stddef.h"
#include "../io/printf.h"
#include "../utils/utils.h"


//denne filen er hentet fra solution guide til Per Arne Andersen, med små endringer.

uint32_t last_alloc = 0;
uint32_t heap_start = 0;
uint32_t heap_end = 0;
uint32_t memory_used = 0;
bool is_initialized = false;

void init_kernel_memory(uint32_t* kernel_end) {
    if (is_initialized) {
        mafiaPrint("Kernel already initialized!\n");
        return;
    }
    is_initialized = true;
    last_alloc = (uint32_t)kernel_end + 0x1000;
    heap_start = last_alloc;
    heap_end = heap_start + MAX_HEAP_SIZE;
    
    memset((void*)heap_start, 0, heap_end - heap_start);
    mafiaPrint("Heap Initialized\n");
}



void* malloc(size_t size) {
    if(!size) return NULL;

    uint8_t* mem = (uint8_t*)heap_start;
    while((uint32_t)mem < last_alloc) {
        
        uint8_t status = *mem;
        uint32_t block_size = *((uint32_t*)(mem + 1));
        
        if(!block_size) goto new_alloc;
        if(status) {
            mem += block_size + 5;
            continue;
        }

        if(block_size >= size) {
            *mem = 1;
            memory_used += size + 5;
            mafiaPrint("allocated %d bytes on address %x\n", size, mem + 5);
            memset(mem + 5, 0, size);
            return (void*)(mem + 5);
        }
        mem += block_size + 5;
    }

new_alloc:
    if(last_alloc + size + 5 >= heap_end) {
        mafiaPrint("Cannot allocate %d bytes! No more memory. Please be less greedy.\n", size);
        return NULL;
    }

    // Skriv header
    *((uint8_t*)last_alloc) = 1; // status
    *((uint32_t*)(last_alloc + 1)) = size; // size
    
    void* ptr = (void*)(last_alloc + 5);
    last_alloc += size + 5;
    memory_used += size + 5;
    
    mafiaPrint("Allocated %d bytes on address 0x%x\n", size, ptr);
    memset(ptr, 0, size);
    return ptr;
}

void free(void* ptr) {
    if(!ptr) return;

    uint8_t* header = (uint8_t*)ptr - 5;
    uint32_t size = *((uint32_t*)(header + 1));
    
    *header = 0; // Marker som frigitt
    memory_used -= size + 5;
    mafiaPrint("Freed %d bytes on address 0x%x\n", size, ptr);
}

void print_memory_layout() {
    mafiaPrint("-----------------------------------------\n");
    mafiaPrint("============  Memory Layout  ============\n");
    mafiaPrint("-----------------------------------------\n");
    mafiaPrint(" Heap start: 0x%x                        \n", heap_start);
    mafiaPrint(" Heap end: 0x%x                          \n", heap_end);
    mafiaPrint(" Heap size: %d MB                        \n", MAX_HEAP_SIZE / (1024 * 1024));
    mafiaPrint(" Memory used: %d bytes                   \n", memory_used);
    mafiaPrint(" Memory available: %d bytes              \n", heap_end - heap_start - memory_used);
    mafiaPrint("-----------------------------------------\n");

}