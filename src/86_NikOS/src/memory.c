#include "memory.h"
#include "libc/string.h"
#include "libc/stdint.h"
#include "terminal.h"

#define HEAP_SIZE 0x800000 // 8 MB

static uint32_t* heap_start;
static uint32_t* heap_end;
static uint32_t* last_allocated;
uint32_t memory_used = 0;

void kernel_memory_init(uint32_t* end) {
    heap_start = (uint32_t*)(((uint32_t)end + 0xFFF) & ~0xFFF);
    heap_end = heap_start + HEAP_SIZE;
    last_allocated = heap_start;
    memset(heap_start, 0, HEAP_SIZE);
}

void* malloc(size_t size) {
    if (!size) return NULL;

    uint32_t* mem = heap_start;

    while (mem < last_allocated) {
        alloc_t* alloc = (alloc_t*)mem;
        if (!alloc->size) break;
        if (alloc->status == 0 && alloc->size >= size) {
            alloc->status = 1;
            return (void*)(mem + sizeof(alloc_t));
        }
        mem += sizeof(alloc_t) + alloc->size;
    }

    if (mem + sizeof(alloc_t) + size > heap_end) {
        return NULL; // out of memory
    }

    alloc_t* alloc = (alloc_t*)last_allocated;
    alloc->size = size;
    alloc->status = 1;

    void* ptr = (void*)(last_allocated + sizeof(alloc_t));
    last_allocated += sizeof(alloc_t) + size;

    memory_used += size + sizeof(alloc_t);

    return ptr;
}

void free(void* ptr) {
    if (!ptr) return;

    alloc_t* alloc = (alloc_t*)((uint32_t)ptr - sizeof(alloc_t));
    memory_used -= alloc->size + sizeof(alloc_t);
    if (alloc->status == 1) {
        alloc->status = 0;
    }
}

void print_memory_layout() {
    terminal_writestring("Memory used: ");
    terminal_writeuint_color(memory_used, get_color(10, 0));
    terminal_writestring(" bytes\n");

    terminal_writestring("Memory free: ");
    terminal_writeuint_color(heap_end - heap_start - memory_used, get_color(10, 0));
    terminal_writestring(" bytes\n");
    
    terminal_writestring("Heap size: ");
    terminal_writeuint_color(heap_end - heap_start, get_color(10, 0));
    terminal_writestring(" bytes\n");

    terminal_writestring("Heap start: ");
    terminal_writeuint_color((uint32_t)heap_start, get_color(10, 0));
    terminal_writestring(" bytes\n");

    terminal_writestring("Heap end: ");
    terminal_writeuint_color((uint32_t)heap_end, get_color(10, 0));
    terminal_writestring(" bytes\n");
}