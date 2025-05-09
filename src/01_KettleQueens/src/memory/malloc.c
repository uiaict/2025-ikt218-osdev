#include "kernel/memory.h"
#include "libc/stdint.h"
#include "printing/terminal.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;

// Simple hex printer
void print_hex(uint32_t val) {
    char buf[11] = "0x00000000";
    const char* hex = "0123456789ABCDEF";
    for (int i = 9; i >= 2; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    terminal_write(buf);
}

// Initialize the kernel memory manager
void init_kernel_memory(uint32_t* kernel_end)
{
    last_alloc = (uint32_t)kernel_end + 0x1000;
    heap_begin = last_alloc;
    pheap_end = 0x400000;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;
    memset((char *)heap_begin, 0, heap_end - heap_begin);
    pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);
    
    terminal_write("Kernel heap starts at ");
    print_hex(last_alloc);
    terminal_write("\n");
}

// Print the current memory layout
void print_memory_layout()
{
    terminal_write("Memory Layout:\n");
    terminal_write("Heap Begin: ");
    print_hex(heap_begin);
    terminal_write("\nHeap End: ");
    print_hex(heap_end);
    terminal_write("\nLast Alloc: ");
    print_hex(last_alloc);
    terminal_write("\nPage Heap Begin: ");
    print_hex(pheap_begin);
    terminal_write("\nPage Heap End: ");
    print_hex(pheap_end);
    terminal_write("\nMemory Used: ");
    print_hex(memory_used);
    terminal_write("\n");
}

// Free a block of memory
void free(void *mem)
{
    alloc_t *alloc = (alloc_t *)((uint8_t*)mem - sizeof(alloc_t));
    memory_used -= alloc->size + sizeof(alloc_t);
    alloc->status = 0;
}

// Free a block of page-aligned memory
void pfree(void *mem)
{
    if((uint32_t)mem < pheap_begin || (uint32_t)mem > pheap_end) return;

    uint32_t ad = (uint32_t)mem;
    ad -= pheap_begin;
    ad /= 4096;

    pheap_desc[ad] = 0;
}

// Allocate a block of page-aligned memory
char* pmalloc(size_t size)
{
    for(int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++)
    {
        if(pheap_desc[i]) continue;
        pheap_desc[i] = 1;

        terminal_write("PAllocated page at ");
        print_hex(pheap_begin + i * 4096);
        terminal_write("\n");

        return (char *)(pheap_begin + i * 4096);
    }

    terminal_write("pmalloc: FATAL: failure!\n");
    return 0;
}

// Allocate a block of memory
void* malloc(size_t size)
{
    if(!size) return 0;

    uint8_t *mem = (uint8_t *)heap_begin;
    while((uint32_t)mem < last_alloc)
    {
        alloc_t *a = (alloc_t *)mem;

        if(!a->size) goto nalloc;
        if(a->status) {
            mem += a->size + sizeof(alloc_t) + 4;
            continue;
        }

        if(a->size >= size)
        {
            a->status = 1;
            memset(mem + sizeof(alloc_t), 0, size);
            memory_used += size + sizeof(alloc_t);
            terminal_write("Reused block at ");
            print_hex((uint32_t)(mem + sizeof(alloc_t)));
            terminal_write("\n");
            return (char *)(mem + sizeof(alloc_t));
        }

        mem += a->size + sizeof(alloc_t) + 4;
    }

nalloc:
    if(last_alloc + size + sizeof(alloc_t) >= heap_end)
    {
        terminal_write("Cannot allocate bytes! Out of memory.\n");
        while (1); // Halt instead of panic
    }

    alloc_t *alloc = (alloc_t *)last_alloc;
    alloc->status = 1;
    alloc->size = size;

    last_alloc += size + sizeof(alloc_t) + 4;
    memory_used += size + sizeof(alloc_t) + 4;

    terminal_write("Allocated new block at ");
    print_hex((uint32_t)alloc + sizeof(alloc_t));
    terminal_write("\n");

    memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
    return (char *)((uint32_t)alloc + sizeof(alloc_t));
}

