#include <memory/memory.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>

alloc_t *heap_start = 0;
alloc_t *heap_end = 0;
uint32_t pheap_start = 0;
uint32_t pheap_end = 0;
uint32_t *pheap_desc = 0;

void init_kernel_memory(uint32_t *kernel_end) {
    heap_start = (alloc_t *)((char *)kernel_end + 0x1000);
    pheap_end = 0x400000; 
    pheap_start = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096); 
    heap_end = pheap_start;
    
    alloc_t *first_block = heap_start;
    first_block->size = pheap_end - (uint32_t)heap_start - sizeof(alloc_t);
    first_block->free = 0; 
    first_block->next = NULL;

    alloc_t *terminal_block = (alloc_t *)heap_end;
    terminal_block->size = 0;
    terminal_block->free = 0;
    terminal_block->next = NULL;

    first_block->next = terminal_block;
}

void print_memory_layout() {
    alloc_t *curr = heap_start;
    printf(0x0F, "Memory Layout:\n");
    while (curr && curr != heap_end) {
        printf(0x0F, "Block at %p: size = %u, free = %d, next: %p\n", curr, curr->size, curr->free, curr->next);
        curr = curr->next;
    }
    printf(0x0F, "End of Memory Layout\n");
}

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    alloc_t *curr = heap_start;
    // alloc_t *prev = NULL;
    alloc_t *best_fit = NULL;

    // Search through the implicit list for a free block
    while (curr && curr != heap_end) {
        if ((curr->free == 0) && (curr->size >= size)) {
            if (!best_fit || (curr->size < best_fit->size)) {
                best_fit = curr;
            }
        }
        // prev = curr;
        curr = curr->next;
    }

    if (!best_fit) {
        return NULL; 
    }

    if (best_fit->size == size) {
        best_fit->free = 1;
        return (char *)best_fit + sizeof(alloc_t);
    }
    
    // If best_fit is too big, split
    if (best_fit->size > size) {
        if (best_fit->size >= size + sizeof(alloc_t)) {
            char *split = (char *)best_fit + sizeof(alloc_t) + size;
            alloc_t *new_block = (alloc_t *)split;
            new_block->size = best_fit->size - (sizeof(alloc_t) + size);
            new_block->free = 0;
            new_block->next = best_fit->next;
            best_fit->size = size;
            best_fit->free = 1;
            best_fit->next = new_block;

            // Return the pointer to the allocated block
            return (void*)((char*)best_fit + sizeof(alloc_t));
        }
        else {
            best_fit->free = 1;
            return (char *)best_fit + sizeof(alloc_t);
        }
        
    }
    return NULL;
}

void free(void *mem) {
    alloc_t *block = (alloc_t *)((char *)mem - sizeof(alloc_t));
    block->free = 0;
    // If the next block is free, merge
    if (block->next && block->next->free == 0) {
        block->size += sizeof(alloc_t) + block->next->size;
        block->next = block->next->next;
    }
    // If the previous block is free, merge
    alloc_t *curr = heap_start;
    while (curr && curr->next != block) {
        curr = curr->next;
    }
    if (curr && curr->free == 0) {
        curr->size += sizeof(alloc_t) + block->size;
        curr->next = block->next;
    }
}

// Paging
void *pmalloc() {
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        if (pheap_desc[i] == 0) {
            pheap_desc[i] = 1;
            uint32_t addr = pheap_start + (i * 4096);
            return (void *)addr;
        }
    }
    return NULL; 
}
void pfree(void *mem) {
    uint32_t addr = (uint32_t)mem;
    if (addr >= pheap_start && addr < pheap_end) {
        int index = (addr - pheap_start) / 4096;
        if (index >= 0 && index < MAX_PAGE_ALIGNED_ALLOCS) {
            pheap_desc[index] = 0;
        }
    }
    else {
        printf(0x0F, "Invalid address to free: %p\n", mem);
    }
}