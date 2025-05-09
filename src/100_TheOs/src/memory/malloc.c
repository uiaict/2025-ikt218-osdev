#include "memory/memory.h"
#include "libc/system.h"
#include "libc/string.h"

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

    pheap_end = 0x164CCF8;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;

    memset((void*)heap_begin, 0, heap_end - heap_begin);

    pheap_desc = (uint8_t*)malloc(MAX_PAGE_ALIGNED_ALLOCS);

    printf("Kernel heap starts at: 0x%x\n", (uint32_t)last_alloc);
}

// Function for printing out nice formated memory information
// Memory in bytes, KB, MB
void print_memory_layout(void) {
    terminal_printf("Memory Information\n");
    terminal_printf("---------------\n");
    if (memory_used < 1024) {
        printf("Memory Used: %d bytes\n", memory_used);
    } else if (memory_used < 1024 * 1024) {
        printf("Memory Used: %d KB\n", memory_used / 1024);
    } else {
        uint32_t mb = memory_used / (1024 * 1024);
        uint32_t decimal = ((memory_used % (1024 * 1024)) * 10) / (1024 * 1024);
        printf("Memory Used: %d.%d MB\n", mb, decimal);
    }
    uint32_t free_mem = heap_end - heap_begin - memory_used;
    if (free_mem < 1024) {
        printf("Free Memory: %d bytes\n", free_mem);
    } else if (free_mem < 1024 * 1024) {
        printf("Free Memory: %d KB\n", free_mem / 1024);
    } else {
        uint32_t mb = free_mem / (1024 * 1024);
        uint32_t decimal = ((free_mem % (1024 * 1024)) * 10) / (1024 * 1024);
        printf("Free Memory: %d.%d MB\n", mb, decimal);
    }
    uint32_t heap_size = heap_end - heap_begin;
    if (heap_size < 1024) {
        printf("Heap Range: 0x%x to 0x%x (%d bytes)\n", heap_begin, heap_end, heap_size);
    } else if (heap_size < 1024 * 1024) {
        printf("Heap Range: 0x%x to 0x%x (%d KB)\n", heap_begin, heap_end, heap_size / 1024);
    } else {
        uint32_t mb = heap_size / (1024 * 1024);
        uint32_t decimal = ((heap_size % (1024 * 1024)) * 10) / (1024 * 1024);
        printf("Heap Range: 0x%x to 0x%x (%d.%d MB)\n", heap_begin, heap_end, mb, decimal);
    }
    
    printf("Page-Aligned Heap: 0x%x to 0x%x\n", pheap_begin, pheap_end);
}
void* malloc(size_t size)
{
    if (!size) return 0;

    uint8_t* current = (uint8_t*)heap_begin;
    while ((uint32_t)current < (uint32_t)last_alloc) {
        alloc_t* block = (alloc_t*)current;

        if (!block->size) break;

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

        current -= block->size + sizeof(alloc_t) + ALIGN_PADDING;
    }

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
    memory_used -= block->size + sizeof(alloc_t) + ALIGN_PADDING;
}

void* pmalloc(size_t size)
{
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        if (pheap_desc[i]) continue;

        pheap_desc[i] = 1;
        uint32_t addr = pheap_begin + i * 4096;
        memset((void*)addr, 0, 4096); 
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
void test_memory(void) {
    printf("Minimal Memory Test\n");
    
    // Store initial value
    uint32_t before = memory_used;
    printf("Initial memory_used: ");
    printf("%d", before);
    printf("\n");
    
    // Do one allocation (1MB)
    void* p = malloc(1024*1024);
    uint32_t after_alloc = memory_used;
    printf("After malloc: ");
    printf("%d", after_alloc);
    printf("\n");
    printf("Bytes added: ");
    printf("%d", after_alloc - before);
    printf("\n");
    
    // Show memory info
    print_memory_layout();

    // Clean memory
    free(p);
    uint32_t after_free = memory_used;
    printf("After free: ");
    printf("%d", after_free);
    printf("\n");
    printf("Bytes removed: ");
    printf("%d", after_alloc - after_free);
    printf("\n");
    printf("Net change: ");
    printf("%d", after_free - before);
    printf("\n");
    
    printf("Test complete\n");
}