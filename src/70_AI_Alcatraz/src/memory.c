#include "memory.h"
#include "printf.h"

// Memory manager state
static memory_block_t* memory_start = NULL;    // Start of memory blocks
static uint32_t heap_start = 0;                // Start address of heap
static uint32_t heap_end = 0;                  // Current end address of heap
static uint32_t heap_max = 0x1000000;          // Maximum heap size (16MB)

// Page directory and page table structures
uint32_t* page_directory = NULL;
uint32_t* first_page_table = NULL;

// Initialize the kernel memory manager
void init_kernel_memory(uint32_t* start_address) {
    // Set up heap starting right after kernel
    heap_start = (uint32_t)start_address;
    heap_start = (heap_start + 0xFFF) & ~0xFFF; // Align to 4KB boundary
    heap_end = heap_start;
    
    memory_start = NULL;
    
    printf("Memory manager initialized: heap starts at 0x%x\n", heap_start);
}

// Print the kernel memory layout
void print_memory_layout() {
    printf("Memory Layout:\n");
    printf("Heap start: 0x%x\n", heap_start);
    printf("Current heap end: 0x%x\n", heap_end);
    printf("Maximum heap: 0x%x\n", heap_max);
    
    // Print allocated blocks
    memory_block_t* current = memory_start;
    int block_count = 0;
    size_t total_allocated = 0;
    size_t total_free = 0;
    
    printf("Memory blocks:\n");
    while (current != NULL) {
        printf("  Block %d: address=0x%x, size=%d bytes, status=%s\n", 
               block_count++, 
               (uint32_t)current + sizeof(memory_block_t),
               current->size,
               current->is_free ? "free" : "allocated");
        
        if (current->is_free)
            total_free += current->size;
        else
            total_allocated += current->size;
            
        current = current->next;
    }
    
    printf("Total memory allocated: %d bytes\n", total_allocated);
    printf("Total memory free: %d bytes\n", total_free);
}

// Find a free block of sufficient size
memory_block_t* find_free_block(memory_block_t** last, size_t size) {
    memory_block_t* current = memory_start;
    
    while (current && !(current->is_free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    
    return current;
}

// Expand the heap to create more space
memory_block_t* expand_heap(memory_block_t* last, size_t size) {
    // Calculate new block size including header
    size_t block_size = sizeof(memory_block_t) + size;
    
    // Check if we've reached the maximum heap size
    if ((heap_end + block_size) > heap_max) {
        printf("ERROR: Out of memory - maximum heap size reached\n");
        return NULL;
    }
    
    // Create a new block
    memory_block_t* block = (memory_block_t*)heap_end;
    heap_end += block_size;
    
    // Initialize the new block
    block->size = size;
    block->is_free = 0;
    block->next = NULL;
    
    // Link the block if we have a previous block
    if (last) {
        last->next = block;
    }
    
    return block;
}

// Split a block if it's much larger than needed
void split_block(memory_block_t* block, size_t size) {
    // Only split if the remainder would be large enough to be useful
    // (at least sizeof(memory_block_t) + some minimum size)
    if (block->size < size + sizeof(memory_block_t) + 8) {
        return;
    }
    
    // Create a new block from the remainder
    memory_block_t* new_block = (memory_block_t*)((uint8_t*)block + sizeof(memory_block_t) + size);
    new_block->size = block->size - size - sizeof(memory_block_t);
    new_block->is_free = 1;
    new_block->next = block->next;
    
    // Update the current block
    block->size = size;
    block->next = new_block;
}

// Allocate memory from the heap
void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Align size to 4-byte boundary for better memory usage
    size = (size + 3) & ~3;
    
    memory_block_t* block;
    
    if (memory_start == NULL) {
        // First allocation, start the heap
        block = expand_heap(NULL, size);
        if (!block) {
            return NULL;
        }
        memory_start = block;
    } else {
        // Find a free block or expand heap if needed
        memory_block_t* last = memory_start;
        block = find_free_block(&last, size);
        
        if (!block) {
            // No suitable block found, expand the heap
            block = expand_heap(last, size);
            if (!block) {
                return NULL;
            }
        } else {
            // Found a block - mark it as allocated
            block->is_free = 0;
            
            // Split the block if it's much larger than needed
            split_block(block, size);
        }
    }
    
    // Return a pointer to the allocated memory
    return (void*)((uint8_t*)block + sizeof(memory_block_t));
}

// Free previously allocated memory
void free(void* ptr) {
    if (!ptr || !memory_start) {
        return;
    }
    
    // Convert the pointer back to the block header
    memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    
    // Mark the block as free
    block->is_free = 1;
    
    // Future enhancement: Merge adjacent free blocks
}

// Page directory and table initialization
void init_paging() {
    // Allocate space for page directory (must be 4KB aligned)
    page_directory = (uint32_t*)malloc(4096);
    uint32_t pd_addr = (uint32_t)page_directory;
    pd_addr = (pd_addr + 0xFFF) & ~0xFFF; // Align to 4KB boundary
    page_directory = (uint32_t*)pd_addr;
    
    // Allocate first page table
    first_page_table = (uint32_t*)malloc(4096);
    uint32_t pt_addr = (uint32_t)first_page_table;
    pt_addr = (pt_addr + 0xFFF) & ~0xFFF; // Align to 4KB boundary
    first_page_table = (uint32_t*)pt_addr;
    
    // Map first 4MB of memory (identity mapping)
    for (uint32_t i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 4096) | 3; // Present + Read/Write
    }
    
    // Set the page directory entry
    page_directory[0] = (uint32_t)first_page_table | 3; // Present + Read/Write
    
    // Clear the rest of the page directory
    for (uint32_t i = 1; i < 1024; i++) {
        page_directory[i] = 0; // Not present
    }
    
    // Enable paging
    asm volatile("mov %0, %%cr3" : : "r" (page_directory)); // Load page directory
    
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000; // Enable paging bit
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
    
    printf("Paging initialized - identity mapped first 4MB\n");
}
