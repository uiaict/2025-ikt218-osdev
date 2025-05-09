#include "memory.h"
#include "terminal.h"
#include "common.h"

// Memory manager state
static uint32_t* heap_start;
static uint32_t* heap_current;
static uint32_t* heap_max;

// Memory block header
typedef struct memory_block {
    size_t size;
    uint8_t is_free;
    struct memory_block* next;
} memory_block_t;

// First block in our linked list
static memory_block_t* first_block = NULL;

// For printing addresses
void print_hex(uint32_t num) {
    char buf[9];
    const char hex_chars[] = "0123456789ABCDEF";
    
    for(int i = 7; i >= 0; i--) {
        buf[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    buf[8] = '\0';
    
    terminal_writestring("0x");
    terminal_writestring(buf);
}

void init_kernel_memory(uint32_t* kernel_end) {
    heap_start = kernel_end;
    heap_current = heap_start;
    
    // 16MB heap
    heap_max = (uint32_t*)(((uint32_t)kernel_end) + 16 * 1024 * 1024);
    
    terminal_writestring("Kernel memory initialized:\n");
    terminal_writestring("  Heap start: ");
    print_hex((uint32_t)heap_start);
    terminal_writestring("\n  Heap max: ");
    print_hex((uint32_t)heap_max);
    terminal_writestring("\n");
    
    first_block = NULL;
}

void init_paging() {
    // Just a placeholder for now
    terminal_writestring("Paging initialized (placeholder)\n");
}

void print_memory_layout() {
    terminal_writestring("Memory Layout:\n");
    
    terminal_writestring("  Kernel code: 0x00100000 - ");
    print_hex((uint32_t)heap_start);
    terminal_writestring("\n  Kernel heap: ");
    print_hex((uint32_t)heap_start);
    terminal_writestring(" - ");
    print_hex((uint32_t)heap_max);
    
    terminal_writestring("\n  Current heap usage: ");
    print_hex((uint32_t)(heap_current - heap_start));
    terminal_writestring(" bytes\n");
    
    // Show allocated blocks
    memory_block_t* current = first_block;
    int block_num = 0;
    while (current != NULL) {
        terminal_writestring("  Block ");
        terminal_putchar('0' + block_num++);
        terminal_writestring(" at ");
        print_hex((uint32_t)current);
        terminal_writestring(", size: ");
        print_hex(current->size);
        terminal_writestring(", free: ");
        if (current->is_free) {
            terminal_writestring("yes\n");
        } else {
            terminal_writestring("no\n");
        }
        current = current->next;
    }
}

void* malloc(size_t size) {
    if (size == 0) return NULL;
    
    // Align size to 4 bytes
    size = (size + 3) & ~3;
    
    // Try to find a free block
    memory_block_t* current = first_block;
    
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            // Found a free block
            current->is_free = 0;
            
            // Split if block is much bigger than needed
            if (current->size > size + sizeof(memory_block_t) + 8) {
                memory_block_t* new_block = (memory_block_t*)((uint32_t)current + sizeof(memory_block_t) + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            return (void*)((uint32_t)current + sizeof(memory_block_t));
        }
        
        current = current->next;
    }
    
    // Allocate a new block
    memory_block_t* block = (memory_block_t*)heap_current;
    uint32_t total_size = size + sizeof(memory_block_t);
    
    // Check if we have enough space
    if ((uint32_t)heap_current + total_size > (uint32_t)heap_max) {
        terminal_writestring("ERROR: Out of memory!\n");
        return NULL;
    }
    
    // Advance heap pointer
    heap_current = (uint32_t*)((uint32_t)heap_current + total_size);
    
    // Initialize the block
    block->size = size;
    block->is_free = 0;
    block->next = NULL;
    
    // Add to linked list
    if (first_block == NULL) {
        first_block = block;
    } else {
        current = first_block;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = block;
    }
    
    return (void*)((uint32_t)block + sizeof(memory_block_t));
}

void free(void* ptr) {
    if (ptr == NULL) return;
    
    // Get the block header
    memory_block_t* block = (memory_block_t*)((uint32_t)ptr - sizeof(memory_block_t));
    
    // Mark the block as free
    block->is_free = 1;
    
    // Try to merge adjacent free blocks
    memory_block_t* current = first_block;
    
    while (current != NULL && current->next != NULL) {
        if (current->is_free && current->next->is_free) {
            // Combine the blocks
            current->size += sizeof(memory_block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}