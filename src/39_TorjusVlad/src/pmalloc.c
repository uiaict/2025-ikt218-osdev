#include "pmalloc.h"
#include "libc/stdio.h"
#include "libc/memory.h"
#define PHEAP_SIZE (MAX_PAGE_ALIGNED_ALLOCS * 4096)  // 32 pages = 128KB


#define MAX_PAGE_ALIGNED_ALLOCS 32

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;


void init_kernel_memory(void* kernel_end) {
    heap_begin = (uint32_t)kernel_end;

    // Align to next page boundary
    if (heap_begin % 4096 != 0)
        heap_begin = (heap_begin + 4095) & ~0xFFF;

    pheap_begin = heap_begin;
    pheap_end = pheap_begin + PHEAP_SIZE;

    heap_begin = pheap_end;
    heap_end = heap_begin + 512 * 1024; // 512 KB

    memory_used = 0;

    // Allocate the descriptor table
    pheap_desc = (uint8_t*)heap_begin;
    heap_begin += MAX_PAGE_ALIGNED_ALLOCS;

    // Zero the descriptor memory
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        pheap_desc[i] = 0;
    }

    size_t heap_size = heap_end - heap_begin;
    heap_init((void*)heap_begin, heap_size);

    printf("Kernel memory initialized\n");
}


// Free a block of page-aligned memory
void pfree(void *mem)
{
    if(mem < pheap_begin || mem > pheap_end) return;

    // Determine the page ID
    uint32_t ad = (uint32_t)mem;
    ad -= pheap_begin;
    ad /= 4096;

    // Set the page descriptor to free
    pheap_desc[ad] = 0;
}

// Allocate a block of page-aligned memory
char* pmalloc(size_t size)
{
    // Loop through the available list
    for(int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++)
    {
        if(pheap_desc[i]) continue;
        pheap_desc[i] = 1;
        printf("PAllocated from 0x%x to 0x%x\n", pheap_begin + i*4096, pheap_begin + (i+1)*4096);
        return (char *)(pheap_begin + i*4096);
    }
    printf("pmalloc: FATAL: failure!\n");
    return 0;
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