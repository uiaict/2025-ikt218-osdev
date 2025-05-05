#include "memory/memory.h"
#include "libc/system.h"
#include "libc/string.h" // for memset

#define MAX_PAGE_ALIGNED_ALLOCS 32
#define ALIGN_PADDING 4

typedef struct {
    uint8_t status;      // 0 = free, 1 = used
    uint32_t size;       // size in bytes
} alloc_t;

// Heap and page-aligned heap
static uint32_t* last_alloc = 0;
static uint32_t heap_begin = 0;
static uint32_t heap_end = 0;
static uint32_t pheap_begin = 0;
static uint32_t pheap_end = 0;
static uint8_t* pheap_desc = 0;

static uint32_t memory_used = 0;

void init_kernel_memory(uint32_t* kernel_end)
{
    last_alloc = (uint32_t*)((uint32_t)kernel_end + 0x1000);
    heap_begin = (uint32_t)last_alloc;

    pheap_end = 0x400000;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;

    memset((void*)heap_begin, 0, heap_end - heap_begin);

    // Reserve space for page-aligned heap bitmap
    pheap_desc = (uint8_t*)malloc(MAX_PAGE_ALIGNED_ALLOCS);

    printf("Kernel heap starts at: 0x%x\n", (uint32_t)last_alloc);
}

void print_memory_layout()
{
    printf("Memory Used: %d bytes\n", memory_used);
    printf("Free Memory: %d bytes\n", heap_end - heap_begin - memory_used);
    printf("Heap Range: 0x%x – 0x%x (%d bytes)\n", heap_begin, heap_end, heap_end - heap_begin);
    printf("Page-Aligned Heap: 0x%x – 0x%x\n", pheap_begin, pheap_end);
}

uint32_t get_memory_used(void) {
    return memory_used;
}
void* malloc(size_t size)
{
    if (!size) return 0;

    uint8_t* current = (uint8_t*)heap_begin;
    while ((uint32_t)current < (uint32_t)last_alloc) {
        alloc_t* block = (alloc_t*)current;

        if (!block->size) break; // Reached uninitialized space

        if (block->status) {
            current += block->size + sizeof(alloc_t) + ALIGN_PADDING;
            continue;
        }

        if (block->size >= size) {
            block->status = 1;
            memset(current + sizeof(alloc_t), 0, size);
            memory_used += size + sizeof(alloc_t);
            return current + sizeof(alloc_t);
        }

        current += block->size + sizeof(alloc_t) + ALIGN_PADDING;
    }

    // No reusable block found, allocate new one
    if ((uint32_t)last_alloc + size + sizeof(alloc_t) >= heap_end) {
        panic("❌ malloc: Out of memory!");
    }

    alloc_t* new_block = (alloc_t*)last_alloc;
    new_block->status = 1;
    new_block->size = size;

    last_alloc = (uint32_t*)((uint32_t)last_alloc + size + sizeof(alloc_t) + ALIGN_PADDING);
    memory_used += size + sizeof(alloc_t) + ALIGN_PADDING;

    memset((uint8_t*)new_block + sizeof(alloc_t), 0, size);
    return (uint8_t*)new_block + sizeof(alloc_t);
}

void free(void* ptr)
{
    if (!ptr) return;
    alloc_t* block = (alloc_t*)((uint8_t*)ptr - sizeof(alloc_t));
    block->status = 0;
    memory_used -= block->size + sizeof(alloc_t);
}

void* pmalloc(size_t size)
{
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        if (pheap_desc[i]) continue;

        pheap_desc[i] = 1;
        uint32_t addr = pheap_begin + i * 4096;
        memset((void*)addr, 0, 4096); // Clean the page
        printf("✅ pmalloc: 0x%x → 0x%x\n", addr, addr + 4096);
        return (void*)addr;
    }

    panic("pmalloc: Out of page-aligned slots!");
    return 0;
}

void pfree(void* ptr)
{
    if (!ptr || (uint32_t)ptr < pheap_begin || (uint32_t)ptr >= pheap_end) return;

    uint32_t page_id = ((uint32_t)ptr - pheap_begin) / 4096;
    pheap_desc[page_id] = 0;
}
