#include "memory/heap.h"
#include "terminal/print.h"

#include "libc/stdint.h"
#include "libc/stddef.h"

typedef struct heap_block {
    size_t size;
    int free;
    struct heap_block* next;
} heap_block_header_t;

// Start of the linked list representing the heap
static heap_block_header_t* heap_start = NULL;

/*
* @brief Initializes the heap memory.
 * @param heap_mem_start The start address of the heap memory.
 * @param heap_size The size of the heap memory.
 * This function sets up the initial block of memory for the heap.
 */
void heap_init (void* heap_mem_start, size_t heap_size) {
    // Initialize the heap start pointer
    heap_start = (heap_block_header_t*)heap_mem_start;

    // Set up the first block
    heap_start->size = heap_size - sizeof(heap_block_header_t); // Leave space for the header
    heap_start->free = 1;
    heap_start->next = NULL;
}

void* malloc (size_t size) {
    heap_block_header_t* current = heap_start;

    while (current) {
        if (current->free && current->size >= size) {
            size_t remaining_size = current->size - size;

            if (remaining_size > sizeof(heap_block_header_t)) {
                heap_block_header_t* new_block = (heap_block_header_t*)((uint8_t*)(current + 1) + size);
                new_block->size = remaining_size - sizeof(heap_block_header_t);
                new_block->free = 1;
                new_block->next = current->next;

                // Update the current block
                current->size = size;
                current->next = new_block;
            }

            current->free = 0;

            // Return the pointer to the memory after the header
            return (void*)(current + 1); 
        }

        current = current->next;
    }
    return NULL; // No suitable block found
}

void free (void* ptr) {
    if (!ptr) return;

    heap_block_header_t* block = (heap_block_header_t*)ptr - 1; // Get the header
    block->free = 1;

    // Coalesce with next blocks
    if (block->next && block->next->free == 1) {
        block->size += sizeof(heap_block_header_t) + block->next->size;
        block->next = block->next->next;
    }

    // Coalesce with previous blocks
    heap_block_header_t* current = heap_start;
    while (current && current->next) {
        if (current->next == block && current->free == 1) {
            current->size += sizeof(heap_block_header_t) + block->size;
            current->next = block->next;
            break;
        }
        current = current->next;
    }
}

void print_heap() {
    heap_block_header_t* current = heap_start;
    int i = 0;
    if (current->next == NULL) {
        printf("Heap is empty. Available space: %u bytes\n\n", (unsigned int)current->size);
        return;
    }
    while (current) {
        printf("Block %d: Block at %p: size=%u bytes, free=%d\n", i, current, (unsigned int)current->size, current->free);
        current = current->next;
        i++;
    }
    printf("\n");
}

