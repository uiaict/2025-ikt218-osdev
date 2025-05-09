#include "memory/memory.h"
#include "memory/memutils.h"
#include "libc/stdio.h"
#include "libc/system.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32 // Max pages allowed

// stores adresses. Adress is value, not pointer
uint32_t last_alloc = 0; // adress before header
uint32_t heap_begin = 0;
uint32_t heap_end = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0; // array storing status of pages
uint32_t memory_used = 0;

// Initialize the kernel memory manager
uint32_t init_kernel_memory(uint32_t* kernel_end){

    last_alloc = kernel_end + 0x1000; // Last alloc right after kernel (no allocs perfomed)
    heap_begin = last_alloc;          // Heap begins right after kernel
    pheap_end = 0x400000;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;

    memset((char*)heap_begin, 0, heap_end - heap_begin); // Sets entire heap to 0
    pheap_desc = (uint8_t*)malloc(MAX_PAGE_ALIGNED_ALLOCS);
    return heap_begin;
}

void print_when_allocating(bool b){
    do_print = b;
}

// Print the current memory layout
void print_memory_layout(){

    printf("Memory used: %d bytes\n\r", memory_used);
    printf("Memory free: %d bytes\n\r", heap_end - heap_begin - memory_used);
    printf("Heap size: %d bytes\n\r", heap_end - heap_begin);
    printf("Heap start: 0x%x\n\r", heap_begin);
    printf("Heap end: 0x%x\n\r", heap_end);
    printf("PHeap start: 0x%x\n\rPHeap end: 0x%x\n\r", pheap_begin, pheap_end);
}


// Allocate a block of memory
void* malloc(size_t size){

    if(size == 0){
        return 0;
    }

    // Loop through blocks to find an available block with enough size
    
    uint8_t *mem = (uint8_t*)heap_begin;

    while((uint32_t)mem < last_alloc){

        alloc_t *a = (alloc_t*)mem;
        if (do_print){printf("mem=0x%x a={.status=%d, .size=%d}\n\r", mem, a->status, a->size);}

        if(!a->size){
            // size=0 means that it has not been allocated
            // you have likely reached last_alloc
            break;
        }
        if(a->status){
            // block is allocated
            mem += sizeof(alloc_t); // skip header
            mem += a->size;         // skip allocated
            mem += 4;               
            continue;
        }

        // block not allocated and big enough
        // adjust its size, set the status, and return the location.
        if(a->size >= size){
            if (do_print){printf("RE:Allocated %d bytes from 0x%x to 0x%x\n\r", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);}

            a->status = 1;
            memset(mem + sizeof(alloc_t), 0, size); // clear the memory needed after the header
            memory_used += size + sizeof(alloc_t);  // update memory used
            return (char *)(mem + sizeof(alloc_t)); // return pointer to allocated memory (after header)
        }

        // block not allocated, but too small
        mem += sizeof(alloc_t); // skip header
        mem += a->size;         // skip block
        mem += 4;               
    }


    if(last_alloc + size + sizeof(alloc_t) >= heap_end){
        // not enough memory for the allocation
        panic("Cannot allocate bytes! Out of memory.\n\r");
    }

    // set header for new alloc
    alloc_t *alloc = (alloc_t *)last_alloc;
    alloc->status = 1;
    alloc->size = size;

    // updates last_alloc
    last_alloc += size;
    last_alloc += sizeof(alloc_t);
    last_alloc += 4;                

    if (do_print){printf("Allocated %d bytes from 0x%x to 0x%x\n\r", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);}

    // updates memory_used, clears memory of new alloc, returns pointer (after header)
    memory_used += size + 4 + sizeof(alloc_t);
    memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
    return (char *)((uint32_t)alloc + sizeof(alloc_t));
}


// Free a block of memory
void free(void *mem){
    // mem points to the start of the memory
    // Step back to get the adress of the header for that alloc
    alloc_t *alloc = (mem - sizeof(alloc_t));
    memory_used -= alloc->size + sizeof(alloc_t);
    alloc->status = 0;
}

// Allocate a block of page-aligned memory
char* pmalloc(size_t size){

    // Loop through the available list
    for(int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++){

        if(pheap_desc[i]){
            continue;
        }
        pheap_desc[i] = 1;
        printf("PAllocated from 0x%x to 0x%x\n\r", pheap_begin + i*4096, pheap_begin + (i+1)*4096);
        return (char *)(pheap_begin + i*4096);
    }
    printf("pmalloc: FATAL: failure!\n\r");
    return 0;
}

// Free a block of page-aligned memory
void pfree(void *mem){

    if(mem < pheap_begin || mem > pheap_end){
         return; // mem not in pheap
    }

    // Determine the page ID
    uint32_t ad = (uint32_t)mem;
    ad -= pheap_begin;
    ad /= 4096;

    // Set the page descriptor to free
    pheap_desc[ad] = 0;
}