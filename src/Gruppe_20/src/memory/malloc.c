#include "memory/memory.h"
#include "libc/system.h"
#include "libc/string.h"
#include "libc/stdio.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32


static uint32_t last_alloc = 0;
static uint32_t heap_end = 0;
static uint32_t heap_begin = 0;
static uint32_t pheap_begin = 0;
static uint32_t pheap_end = 0;
static uint8_t *pheap_desc = 0;
static uint32_t memory_used = 0;

void init_kernel_memory(uint32_t* kernel_end) {
    last_alloc = (uint32_t)(kernel_end) + 0x1000;
    heap_begin = last_alloc;
    pheap_end = 0x400000;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;
    memset((char *)heap_begin, 0, heap_end - heap_begin);
    pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);
    printf("Kernel heap starts at 0x%x\n", last_alloc);
}

void* malloc(size_t size) {
    if(!size) return NULL;

    uint8_t *mem = (uint8_t *)heap_begin;
    while((uint32_t)mem < last_alloc) {
        alloc_t *a = (alloc_t *)mem;
        if(!a->size) break;
        
        if(!a->status && a->size >= size) {
            a->status = 1;
            memory_used += size + sizeof(alloc_t);
            memset(mem + sizeof(alloc_t), 0, size);
            return (void*)(mem + sizeof(alloc_t));
        }
        mem += a->size + sizeof(alloc_t) + 4;
    }

    if(last_alloc + size + sizeof(alloc_t) >= heap_end) {
        panic("Out of memory");
    }

    alloc_t *alloc = (alloc_t *)last_alloc;
    alloc->status = 1;
    alloc->size = size;

    last_alloc += size + sizeof(alloc_t) + 4;
    memory_used += size + sizeof(alloc_t) + 4;
    memset((void*)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
    return (void*)((uint32_t)alloc + sizeof(alloc_t));
}
// Print the current memory layout
void print_memory_layout()
{
    printf("Memory used: %d bytes\n", memory_used);
    printf("Memory free: %d bytes\n", heap_end - heap_begin - memory_used);
    printf("Heap size: %d bytes\n", heap_end - heap_begin);
    printf("Heap start: 0x%x\n", heap_begin);
    printf("Heap end: 0x%x\n", heap_end);
    printf("PHeap start: 0x%x\nPHeap end: 0x%x\n", pheap_begin, pheap_end);
}
void free(void *mem) {
    if (!mem) return;
    alloc_t *alloc = (alloc_t*)((uint8_t*)mem - sizeof(alloc_t));
    memory_used -= alloc->size + sizeof(alloc_t);
    alloc->status = 0;
}