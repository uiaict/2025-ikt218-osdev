#define _SIZE_T_DEFINED  // Sett før vi inkluderer andre filer
#include "libc/stdint.h"
#include "memory_manager.h"
#include "display.h"

// Definer NULL hvis den ikke er definert
#ifndef NULL
#define NULL ((void*)0)
#endif

// Defines for memory management
#define PAGE_SIZE 4096  // 4KB pages
#define HEAP_START 0x400000  // 4MB - start of heap
#define HEAP_INITIAL_SIZE 0x100000  // 1MB initial heap size

// Memory block structure for tracking allocated blocks
typedef struct memory_block {
    long unsigned int size;        // Size of this block (including header)
    uint8_t is_free;               // 1 if block is free, 0 if allocated
    struct memory_block* next;     // Next block in the list
} memory_block_t;

// Global variables for memory management
static memory_block_t* heap_start = NULL;
static uint32_t heap_end = 0;
static uint32_t kernel_end = 0;

// Page directory and tables for paging
static uint32_t* page_directory = NULL;
static uint8_t paging_enabled = 0;

/**
 * Initialize the kernel memory manager
 * 
 * This function initializes the kernel heap, starting at the end of the kernel.
 * It sets up the initial free block of memory.
 * 
 * @param addr Pointer to the end of the kernel in memory (from linker script)
 */
void init_kernel_memory(uint32_t* addr) {
    display_write_color("Initializing Kernel Memory Manager...\n", COLOR_WHITE);
    
    // Store the kernel end address
    kernel_end = (uint32_t)addr;
    
    // Align the heap start to a page boundary (4KB)
    uint32_t aligned_start = (kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    
    // Ensure heap start is at least 4MB
    if (aligned_start < HEAP_START) {
        aligned_start = HEAP_START;
    }
    
    // Set the heap start and end
    heap_start = (memory_block_t*)aligned_start;
    heap_end = aligned_start + HEAP_INITIAL_SIZE;
    
    // Ensure heap_end is page aligned
    heap_end = (heap_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    
    // Validate heap boundaries
    if ((uint32_t)heap_start >= heap_end) {
        display_write_color("ERROR: Invalid heap boundaries\n", COLOR_LIGHT_RED);
        return;
    }
    
    // Create the initial free block
    heap_start->size = heap_end - aligned_start - sizeof(memory_block_t);
    heap_start->is_free = 1;
    heap_start->next = NULL;
    
    display_write_color("\n=== Memory Layout Information ===\n", COLOR_YELLOW);
    display_write_color("Kernel End Address: 0x", COLOR_WHITE);
    display_write_hex(kernel_end);
    display_write("\n");
    
    display_write_color("Heap Start Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)heap_start);
    display_write("\n");
    
    display_write_color("Heap End Address: 0x", COLOR_WHITE);
    display_write_hex(heap_end);
    display_write("\n");
    
    display_write_color("Current Heap Size: ", COLOR_WHITE);
    display_write_decimal(heap_end - (uint32_t)heap_start);
    display_write(" bytes\n");
    
    display_write_color("Paging Status: ", COLOR_WHITE);
    if (paging_enabled) {
        display_write_color("Enabled\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("Disabled\n", COLOR_YELLOW);
    }
    
    display_write_color("Page Directory Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_directory);
    display_write("\n");
    
    display_write_color("\n=== Memory Allocation Blocks ===\n", COLOR_YELLOW);
    memory_block_t* current = heap_start;
    int block_count = 1;
    
    while (current) {
        display_write_color("Block ", COLOR_WHITE);
        display_write_decimal(block_count++);
        display_write_color(": Address: 0x", COLOR_WHITE);
        display_write_hex((uint32_t)current);
        display_write_color(", Size: ", COLOR_WHITE);
        display_write_decimal(current->size);
        display_write(" bytes");
        display_write_color(", Status: ", COLOR_WHITE);
        if (current->is_free) {
            display_write_color("Free\n", COLOR_LIGHT_GREEN);
        } else {
            display_write_color("Allocated\n", COLOR_LIGHT_RED);
        }
        current = current->next;
    }
    
    display_write("\n");
}

/**
 * Find a free block of memory of the requested size
 * 
 * @param size The size of memory needed (in bytes)
 * @return Pointer to a suitable free block, or NULL if none found
 */
memory_block_t* find_free_block(long unsigned int size) {
    memory_block_t* current = heap_start;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; // No suitable free block found
}

/**
 * Split a block if it's much larger than the requested size
 * 
 * @param block The block to split
 * @param size The requested size
 */
void split_block(memory_block_t* block, long unsigned int size) {
    // Validate parameters
    if (!block || size == 0 || size > block->size) {
        return;
    }
    
    // Only split if the remainder would be big enough for a new block
    // (at least sizeof(memory_block_t) + 32 bytes for data)
    if (block->size >= size + sizeof(memory_block_t) + 32) {
        memory_block_t* new_block = (memory_block_t*)((uint8_t*)block + sizeof(memory_block_t) + size);
        
        // Validate that the new block address is within our heap bounds
        if ((uint32_t)new_block < heap_end - sizeof(memory_block_t)) {
            // Set up the new block
            new_block->size = block->size - size - sizeof(memory_block_t);
            new_block->is_free = 1;
            new_block->next = block->next;
            
            // Update the current block
            block->size = size;
            block->next = new_block;
        }
    }
}

/**
 * Allocate memory from the kernel heap
 * 
 * @param size Size of memory to allocate in bytes
 * @return Pointer to allocated memory or NULL if allocation failed
 */
void* malloc(long unsigned int size) {
    if (size == 0) {
        display_write_color("WARNING: Attempted to allocate 0 bytes\n", COLOR_YELLOW);
        return NULL;
    }
    
    if (size > HEAP_INITIAL_SIZE - sizeof(memory_block_t)) {
        display_write_color("ERROR: Requested allocation size too large\n", COLOR_LIGHT_RED);
        return NULL;
    }
    
    // Round up size to multiple of 4 bytes for alignment
    size = (size + 3) & ~3;
    
    // Find a free block that's large enough
    memory_block_t* block = find_free_block(size);
    
    if (!block) {
        display_write_color("ERROR: No suitable free block found\n", COLOR_LIGHT_RED);
        return NULL;
    }
    
    // Validate block address
    if ((uint32_t)block < (uint32_t)heap_start || 
        (uint32_t)block >= heap_end - sizeof(memory_block_t)) {
        display_write_color("ERROR: Invalid block address in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    
    // Validate block size
    if (block->size < size || block->size > HEAP_INITIAL_SIZE) {
        display_write_color("ERROR: Invalid block size in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    
    // Mark the block as allocated
    block->is_free = 0;
    
    // Split the block if it's much larger than needed
    split_block(block, size);
    
    // Return a pointer to the memory after the block header
    void* result = (void*)((uint8_t*)block + sizeof(memory_block_t));
    
    // Validate the returned pointer
    if ((uint32_t)result < (uint32_t)heap_start || 
        (uint32_t)result >= heap_end) {
        display_write_color("ERROR: Invalid pointer generated in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    
    return result;
}

/**
 * Merge adjacent free blocks
 * 
 * @param block The starting block to check for merges
 */
void merge_free_blocks(memory_block_t* block) {
    while (block && block->next) {
        if (block->is_free && block->next->is_free) {
            // Calculate total size including the header of the next block
            block->size += sizeof(memory_block_t) + block->next->size;
            
            // Skip the next block
            block->next = block->next->next;
        } else {
            // Move to the next block
            block = block->next;
        }
    }
}

/**
 * Free previously allocated memory
 * 
 * @param ptr Pointer to memory that was previously allocated with malloc
 */
void free(void* ptr) {
    if (ptr == NULL) {
        display_write_color("WARNING: Attempted to free NULL pointer\n", COLOR_YELLOW);
        return;
    }
    
    // Get the block header (located before the data)
    memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    
    // Validate block address
    if ((uint32_t)block < (uint32_t)heap_start || 
        (uint32_t)block >= heap_end - sizeof(memory_block_t)) {
        display_write_color("ERROR: Invalid pointer passed to free()\n", COLOR_LIGHT_RED);
        return;
    }
    
    // Validate block size
    if (block->size == 0 || block->size > HEAP_INITIAL_SIZE) {
        display_write_color("ERROR: Corrupted memory block detected in free()\n", COLOR_LIGHT_RED);
        return;
    }
    
    // Validate block is currently allocated
    if (block->is_free) {
        display_write_color("WARNING: Attempted to free already freed memory\n", COLOR_YELLOW);
        return;
    }
    
    // Mark the block as free
    block->is_free = 1;
    
    // Merge adjacent free blocks
    merge_free_blocks(heap_start);
}

/**
 * Initialize paging for the kernel
 * 
 * Sets up paging with identity mapping for the first 8MB of memory.
 */
void init_paging(void) {
    display_write_color("Initializing paging...\n", COLOR_WHITE);
    
    // Allocate memory for the page directory
    page_directory = (uint32_t*)((kernel_end + PAGE_SIZE) & ~(PAGE_SIZE - 1));
    display_write_color("Page Directory will be at: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_directory);
    display_write("\n");
    
    // Clear the page directory
    display_write_color("Clearing page directory...\n", COLOR_WHITE);
    for (int i = 0; i < 1024; i++) {
        // Set as not present, read/write enabled
        page_directory[i] = 0x00000002;
    }
    
    // Create two page tables for the first 8MB of memory
    display_write_color("Creating page tables...\n", COLOR_WHITE);
    uint32_t* page_table_0_4mb = (uint32_t*)((uint32_t)page_directory + PAGE_SIZE);
    uint32_t* page_table_4_8mb = (uint32_t*)((uint32_t)page_table_0_4mb + PAGE_SIZE);
    
    display_write_color("Page Table 0-4MB will be at: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_table_0_4mb);
    display_write("\n");
    
    display_write_color("Page Table 4-8MB will be at: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_table_4_8mb);
    display_write("\n");
    
    // Identity map the first 4MB of physical memory
    display_write_color("Mapping first 4MB of memory...\n", COLOR_WHITE);
    for (int i = 0; i < 1024; i++) {
        // Present, read/write, page address is i*4KB
        page_table_0_4mb[i] = (i * PAGE_SIZE) | 3;
    }
    
    // Identity map the second 4MB of physical memory
    display_write_color("Mapping second 4MB of memory...\n", COLOR_WHITE);
    for (int i = 0; i < 1024; i++) {
        // Present, read/write, page address is (i + 1024)*4KB
        page_table_4_8mb[i] = ((i + 1024) * PAGE_SIZE) | 3;
    }
    
    // Set up the page directory entries
    display_write_color("Setting up page directory entries...\n", COLOR_WHITE);
    // First 4MB - page_table_0_4mb
    page_directory[0] = ((uint32_t)page_table_0_4mb) | 3;
    // Second 4MB - page_table_4_8mb
    page_directory[1] = ((uint32_t)page_table_4_8mb) | 3;
    
    display_write_color("Loading page directory into CR3...\n", COLOR_WHITE);
    // Load page directory into CR3
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_directory));
    
    display_write_color("Enabling paging bit in CR0...\n", COLOR_WHITE);
    // Enable paging by setting PG bit (bit 31) in CR0
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
    
    // Add a small delay to ensure paging is enabled
    delay(100);
    
    // Verify paging is enabled by checking CR0
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    if (cr0 & 0x80000000) {
        display_write_color("Paging is now enabled!\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("ERROR: Failed to enable paging!\n", COLOR_LIGHT_RED);
        return;
    }
    
    paging_enabled = 1;
    display_write_color("Paging initialized successfully.\n", COLOR_LIGHT_GREEN);
}

/**
 * Print memory layout information
 */
void print_memory_layout(void) {
    display_write_color("=== Memory Layout Information ===\n", COLOR_YELLOW);
    
    display_write_color("Kernel End Address: 0x", COLOR_WHITE);
    display_write_hex(kernel_end);
    display_write("\n");
    
    display_write_color("Heap Start Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)heap_start);
    display_write("\n");
    
    display_write_color("Heap End Address: 0x", COLOR_WHITE);
    display_write_hex(heap_end);
    display_write("\n");
    
    display_write_color("Current Heap Size: ", COLOR_WHITE);
    display_write_decimal(heap_end - (uint32_t)heap_start);
    display_write(" bytes\n");
    
    display_write_color("Paging Status: ", COLOR_WHITE);
    if (paging_enabled) {
        display_write_color("Enabled\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("Disabled\n", COLOR_LIGHT_RED);
    }
    
    display_write_color("Page Directory Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_directory);
    display_write("\n");
    
    // Print memory block information
    display_write_color("\n=== Memory Allocation Blocks ===\n", COLOR_YELLOW);
    memory_block_t* current = heap_start;
    int block_count = 0;
    int free_blocks = 0;
    long unsigned int free_memory = 0;
    
    while (current) {
        block_count++;
        if (current->is_free) {
            free_blocks++;
            free_memory += current->size;
        }
        
        display_write_color("Block ", COLOR_WHITE);
        display_write_decimal(block_count);
        display_write(": ");
        
        display_write_color("Address: 0x", COLOR_WHITE);
        display_write_hex((uint32_t)current);
        display_write(", ");
        
        display_write_color("Size: ", COLOR_WHITE);
        display_write_decimal(current->size);
        display_write(" bytes, ");
        
        if (current->is_free) {
            display_write_color("Status: Free\n", COLOR_LIGHT_GREEN);
        } else {
            display_write_color("Status: Allocated\n", COLOR_LIGHT_RED);
        }
        
        current = current->next;
        
        // Limit the number of blocks displayed to avoid flooding the screen
        if (block_count >= 10) {
            display_write_color("... more blocks not shown ...\n", COLOR_GRAY);
            break;
        }
    }
    
    display_write_color("\nTotal Blocks: ", COLOR_WHITE);
    display_write_decimal(block_count);
    display_write("\n");
    
    display_write_color("Free Blocks: ", COLOR_WHITE);
    display_write_decimal(free_blocks);
    display_write("\n");
    
    display_write_color("Free Memory: ", COLOR_WHITE);
    display_write_decimal(free_memory);
    display_write(" bytes\n");
    
    display_write_color("==============================\n", COLOR_YELLOW);
} 