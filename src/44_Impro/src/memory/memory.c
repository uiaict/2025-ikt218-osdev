#include "memory.h"
#include "util.h"
#include "vga.h"
#include "libc/stdint.h"
#include "libc/stdio.h"

#define HEAP_SIZE (1024 * 1024) //1MB


typedef struct block_meta {
    uint32_t size;
    uint8_t free;
    struct block_meta* next;
} block_meta_t;

#define META_SIZE sizeof(block_meta_t)

static uint8_t* heap_start = 0;
static uint8_t* heap_end = 0;
static block_meta_t* base = 0;

void init_kernel_memory(uint32_t* kernel_end) {
    heap_start = (uint8_t*)kernel_end;
    heap_end = heap_start + HEAP_SIZE;

    base = (block_meta_t*)heap_start;
    base->size = HEAP_SIZE - META_SIZE;
    base->free = 1;
    base->next = 0;
}

block_meta_t* find_free_block(block_meta_t** last, uint32_t size) {
    block_meta_t* current = base;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

block_meta_t* request_space(block_meta_t* last, uint32_t size) {

    uint8_t* potential = (uint8_t*)last + META_SIZE + last->size;
    if (potential + META_SIZE > heap_end) return 0;

    block_meta_t* block = (block_meta_t*)potential;
    block->size = size;
    block->free = 0;
    block->next = 0;

    last->next = block;
    return block;
}

void split_block(block_meta_t* block, uint32_t size) {
    if (block->size <= size + META_SIZE) return; 

    block_meta_t* new_block = (block_meta_t*)((uint8_t*)block + META_SIZE + size);
    new_block->size = block->size - size - META_SIZE;
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}


void* malloc(uint32_t size) {
    if (size == 0) return 0;

    size = (size + 3) & ~0x3;

    block_meta_t* block;
    if (!base) return 0;

    block_meta_t* last = base;
    block = find_free_block(&last, size);

    if (!block) {
        block = request_space(last, size);
        if (!block) return 0;
    } else {
        split_block(block, size);
        block->free = 0;
    }

    return (void*)(block + 1);
}

block_meta_t* get_block_ptr(void* ptr) {
    return (block_meta_t*)ptr - 1;
}

void free(void* ptr) {
    if (!ptr) return;

    block_meta_t* block = get_block_ptr(ptr);
    block->free = 1;
}



void print_memory_layout() {
    block_meta_t* current = base;

    printf("Heap layout:\n");
    while (current) {
        printf("Block at: %d", (uint32_t)current);
        printf(" | Size: %d", current->size);
        
        printf(" | free: ");
        if (current->free) {
            printf("yes");
        } else {
            printf("no");
        }

        printf("\n\r");

        current = current->next;
    }
}
