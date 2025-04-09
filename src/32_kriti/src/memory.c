#include "memory.h"
#include "kprint.h"

// Memory management globals
static mem_block_t *heap_start = NULL;
static unsigned long heap_end = 0;
static size_t total_memory = 0;
static size_t used_memory = 0;

// 32-bit paging structures
typedef struct {
    unsigned long entries[1024];
} page_directory_t;

typedef struct {
    unsigned long entries[1024];
} page_table_t;

// Page directory (must be 4KB aligned)
__attribute__((aligned(4096))) static page_directory_t kernel_page_directory;

// First page table (must be 4KB aligned)
__attribute__((aligned(4096))) static page_table_t kernel_page_table;

// Initialize the kernel memory manager
void init_kernel_memory(unsigned long *start_addr) {
    // Set up initial heap area
    heap_start = (mem_block_t*)start_addr;
    
    // Configure the first block to span the remaining memory
    // In a real OS, you'd determine this from multiboot info
    // For simplicity, we'll assume 128MB total memory
    total_memory = 128 * 1024 * 1024; // 128MB
    
    // Set heap end to a reasonable limit for now
    heap_end = (unsigned long)start_addr + total_memory;
    
    // Initialize first memory block
    heap_start->size = total_memory - sizeof(mem_block_t);
    heap_start->free = true;
    heap_start->next = NULL;
    
    kprint("Kernel memory manager initialized\n");
    kprint("Start address: 0x");
    kprint_hex((unsigned long)start_addr);
    kprint("\nHeap size: ");
    kprint_dec(heap_start->size);
    kprint(" bytes\n");
}

// Print memory layout information
void print_memory_layout(void) {
    kprint("Memory Layout Information:\n");
    kprint("-------------------------\n");
    
    kprint("Total Memory: ");
    kprint_dec(total_memory);
    kprint(" bytes (");
    kprint_dec(total_memory / 1024);
    kprint(" KB)\n");
    
    kprint("Used Memory: ");
    kprint_dec(used_memory);
    kprint(" bytes (");
    kprint_dec(used_memory / 1024);
    kprint(" KB)\n");
    
    kprint("Free Memory: ");
    kprint_dec(total_memory - used_memory);
    kprint(" bytes (");
    kprint_dec((total_memory - used_memory) / 1024);
    kprint(" KB)\n");
    
    kprint("Heap Start: 0x");
    kprint_hex((unsigned long)heap_start);
    kprint("\n");
    
    kprint("Heap End: 0x");
    kprint_hex(heap_end);
    kprint("\n");
    
    // Print memory block information
    mem_block_t *current = heap_start;
    int block_count = 0;
    
    kprint("\nMemory Blocks:\n");
    while (current != NULL) {
        kprint("Block ");
        kprint_dec(block_count++);
        kprint(": Address=0x");
        kprint_hex((unsigned long)current);
        kprint(", Size=");
        kprint_dec(current->size);
        kprint(", Status=");
        kprint(current->free ? "Free" : "Used");
        kprint("\n");
        
        current = current->next;
    }
}

// Find a suitable memory block for allocation
static mem_block_t *find_free_block(size_t size) {
    mem_block_t *current = heap_start;
    
    while (current != NULL) {
        if (current->free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Memory allocation function
void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Align size to 8-byte boundary for 64-bit architecture
    size = (size + 7) & ~7;
    
    // Find a free block
    mem_block_t *block = find_free_block(size);
    
    if (block == NULL) {
        kprint("malloc: out of memory\n");
        return NULL;
    }
    
    // Check if we should split this block
    if (block->size >= size + sizeof(mem_block_t) + 8) {
        // Create a new block after this allocation
        mem_block_t *new_block = (mem_block_t*)((char*)block + sizeof(mem_block_t) + size);
        new_block->size = block->size - size - sizeof(mem_block_t);
        new_block->free = true;
        new_block->next = block->next;
        
        // Update current block
        block->size = size;
        block->next = new_block;
    }
    
    // Mark block as used
    block->free = false;
    
    // Update memory usage stats
    used_memory += block->size + sizeof(mem_block_t);
    
    // Return pointer to usable memory (after the header)
    return (void*)((unsigned long)block + sizeof(mem_block_t));
}

// Free a memory allocation
void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    
    // Get the block header
    mem_block_t *block = (mem_block_t*)((unsigned long)ptr - sizeof(mem_block_t));
    
    // Mark the block as free
    block->free = true;
    
    // Update memory usage stats
    used_memory -= block->size + sizeof(mem_block_t);
    
    // Attempt to coalesce with the next block if it's free
    if (block->next != NULL && block->next->free) {
        block->size += sizeof(mem_block_t) + block->next->size;
        block->next = block->next->next;
    }
    
    // We could also coalesce with the previous block, but that would require a doubly-linked list
    // For simplicity, we're skipping that optimization
}

// Initialize paging system
void init_paging(void) {
    // Clear the page directory
    for (int i = 0; i < 1024; i++) {
        // Set all pages as not present
        kernel_page_directory.entries[i] = 0x00000002; // Supervisor, read/write, not present
    }
    
    // Identity map the first 4MB of memory (kernel space)
    for (int i = 0; i < 1024; i++) {
        unsigned long page_addr = i * 0x1000; // 4KB pages
        kernel_page_table.entries[i] = page_addr | 0x3; // Supervisor, read/write, present
    }
    
    // Map the first page table into the directory
    kernel_page_directory.entries[0] = ((unsigned long)&kernel_page_table) | 0x3; // Supervisor, read/write, present
    
    // Load page directory address into CR3
    __asm__ volatile (
        "movl %0, %%cr3" : : "r"(&kernel_page_directory)
    );
    
    // Enable paging (set bit 31 in CR0)
    unsigned long cr0;
    __asm__ volatile (
        "movl %%cr0, %0" : "=r"(cr0)
    );
    cr0 |= 0x80000000; // Set bit 31
    __asm__ volatile (
        "movl %0, %%cr0" : : "r"(cr0)
    );
    
    kprint("32-bit paging initialized\n");
}

// Get total memory size
size_t get_total_memory(void) {
    return total_memory;
}

// Get used memory size
size_t get_used_memory(void) {
    return used_memory;
}

// Get free memory size
size_t get_free_memory(void) {
    return total_memory - used_memory;
}

#ifdef __cplusplus
// C++ operator new implementation (if needed)
extern "C" void* operator new(size_t size) {
    return malloc(size);
}

// C++ operator delete implementation (if needed)
extern "C" void operator delete(void* ptr) {
    free(ptr);
}

// C++ array versions
extern "C" void* operator new[](size_t size) {
    return malloc(size);
}

extern "C" void operator delete[](void* ptr) {
    free(ptr);
}
#endif