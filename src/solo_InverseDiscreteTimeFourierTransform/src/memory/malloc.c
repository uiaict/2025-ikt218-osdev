#include "memory.h"
#include "libc/system.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "terminal.h"

// Quick way to fix a bug: Fails to find the uintptr_t type from libc/stdint.h
// This is a workaround to define uintptr_t as unsigned int
typedef unsigned int uintptr_t;

char* hex32_to_str(char buf[9], unsigned int val) {
    static const char* hex = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    buf[8] = '\0';
    return buf;
}


#define MAX_PAGE_ALIGNED_ALLOCS 32
#define KERNEL_HEAP_OFFSET      0x1000   // Offset after kernel image to start heap
#define PHEAP_FIXED_END         0x400000 // Fixed end address for the page-aligned heap
#define PAGE_SIZE               4096     // Size of a memory page
#define MEM_BLOCK_PADDING       4        // Padding bytes after each main heap allocation block

uint32_t last_alloc = 0;    // Points after the last allocated block in the main heap
uint32_t heap_end = 0;      // Upper boundary (exclusive) of the main heap
uint32_t heap_begin = 0;    // Lower boundary (inclusive) of the main heap
uint32_t pheap_begin = 0;   // Lower boundary (inclusive) of the page-aligned heap
uint32_t pheap_end = 0;     // Upper boundary (exclusive) of the page-aligned heap
uint8_t *pheap_desc = NULL; // Status descriptors for pheap pages. 0 = free, 1 = used
uint32_t memory_used = 0;   // Total bytes used in main heap



// initializes the kernels heap regions
void init_kernel_memory(uint32_t *kernel_end_addr) {


    // start main heap after kernel image + offset
    last_alloc = (uint32_t)kernel_end_addr + KERNEL_HEAP_OFFSET;
    
    heap_begin = last_alloc;

    // Define page-aligned heap (pheap) region
    pheap_end = PHEAP_FIXED_END;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * PAGE_SIZE);

    // main heap ends where pheap begins
    heap_end = pheap_begin;

    // zero out the main heap region
    if (heap_end > heap_begin) {

        memset((char *)heap_begin, 0, heap_end - heap_begin);
    }

    // allocate memory for pheap descriptors from main heap
    pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);

    {
        char buf[9];
        hex32_to_str(buf, heap_begin);
        terminal_write("Kernel heap starts at 0x");
        terminal_write(buf);
        terminal_write("\n");
    }


}



// Prints current memory layout statistics
void print_memory_layout(void) {
    char buf[12];
  
    // Memory used
    hex32_to_str(buf, memory_used);
    terminal_write("Memory used: ");
    terminal_write(buf);
    terminal_write(" bytes\n");
  
    // Memory free
    hex32_to_str(buf, (heap_end - heap_begin) - memory_used);
    terminal_write("Memory free: ");
    terminal_write(buf);
    terminal_write(" bytes\n");
  
    // Heap start
    hex32_to_str(buf, heap_begin);
    terminal_write("Heap start: 0x");
    terminal_write(buf);
    terminal_write("\n");
  
    // Heap end
    hex32_to_str(buf, heap_end);
    terminal_write("Heap end: 0x");
    terminal_write(buf);
    terminal_write("\n");
  
    // PHeap start/end
    hex32_to_str(buf, pheap_begin);
    terminal_write("PHeap start: 0x");
    terminal_write(buf);
    terminal_write("\n");
  
    hex32_to_str(buf, pheap_end);
    terminal_write("PHeap end:   0x");
    terminal_write(buf);
    terminal_write("\n");
  }
  


// Frees a block in main heap. 'mem' points to user data
void free(void *mem) {

    if (!mem) return;

    // Get header for this block
    alloc_t *alloc = (alloc_t *)((uint8_t*)mem - sizeof(alloc_t));


    if (memory_used >= (alloc->size + sizeof(alloc_t) + MEM_BLOCK_PADDING)) {
        memory_used -= (alloc->size + sizeof(alloc_t) + MEM_BLOCK_PADDING);
    } else {
        memory_used = 0;
    }
    alloc->status = 0;
}


// Frees a page-aligned block from pheap
void pfree(void *mem) {

    if (!mem) return;

    uint32_t mem_addr = (uint32_t)mem;
    if (mem_addr < pheap_begin || mem_addr >= pheap_end) return;

    uint32_t page_index = (mem_addr - pheap_begin) / PAGE_SIZE;

    if (page_index < MAX_PAGE_ALIGNED_ALLOCS) {
        pheap_desc[page_index] = 0;
    } else {
    }
}



// Allocates one page-aligned block from pheap
char *pmalloc(size_t size __attribute__((unused))) {


    if (!pheap_desc) {
        terminal_write("pmalloc: FATAL: pheap_desc not initialized!\n");
        return NULL;
    }

    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {

        if (pheap_desc[i] == 0) { // If page is free
            pheap_desc[i] = 1;    // Mark as used
            uint32_t alloc_addr = pheap_begin + (i * PAGE_SIZE);
            return (char *)alloc_addr;
        }
    }
    terminal_write("pmalloc: No free page available.\n");
    return NULL;
}

// Allocates 'size' bytes from the main heap
void *malloc(size_t size) {

    if (!size) return NULL; // Cannot allocate 0 bytes

    uint8_t *current_block_ptr = (uint8_t *)heap_begin;


    while ((uint32_t)current_block_ptr < last_alloc) {
        
        alloc_t *header = (alloc_t *)current_block_ptr;


        if (header->size == 0 && header->status == 0) {
            goto new_allocation;
        }

        if (header->status == 1) { // Block is used
            // Move to next block
            current_block_ptr += sizeof(alloc_t) + header->size + MEM_BLOCK_PADDING;
            continue;
        }

        // Block is free (header->status == 0)
        if (header->size >= size) { // Found a  large enough free block
            header->status = 1;     // mark as used
            uintptr_t user_mem_ptr = (uintptr_t)current_block_ptr + sizeof(alloc_t);

            memory_used += header->size + sizeof(alloc_t) + MEM_BLOCK_PADDING;

            memset((void*)user_mem_ptr, 0, size); // Zero out the requested part
            return (void*)user_mem_ptr;
        }

        // Free block is too small, move to next block
        current_block_ptr += sizeof(alloc_t) + header->size + MEM_BLOCK_PADDING;
    }

new_allocation:;
    // --- Allocate a new block at the end of the heap ---
    uint32_t total_needed_size = sizeof(alloc_t) + size + MEM_BLOCK_PADDING;
    if (last_alloc + total_needed_size > heap_end) { // Check if new block exceeds heap boundary

        terminal_write("malloc: Noo more space in heap.");
        return NULL;
    }

    alloc_t *new_header = (alloc_t *)last_alloc;
    new_header->status = 1;
    new_header->size = size;

    uintptr_t user_mem_ptr = last_alloc + sizeof(alloc_t);
    last_alloc += total_needed_size; // Advance last_alloc to end of new block

    memory_used += total_needed_size;

    memset((void*)user_mem_ptr, 0, size);
    return (void*)user_mem_ptr;
}