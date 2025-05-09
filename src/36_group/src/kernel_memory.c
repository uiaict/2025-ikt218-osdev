#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include "memory.h"
#include "kernel_memory.h"
#include "io.h"

extern uint32_t start;
extern uint32_t end;

static mem_block_t *heap_head = NULL;
static uintptr_t kernel_heapS;
static uintptr_t kernel_heapE;
static uintptr_t kernel_heapC;

void init_kernel_memory(void *kernel_end)
{
    kernel_heapS = (uintptr_t)kernel_end;
    kernel_heapE = kernel_heapS + 0x1000000;
    kernel_heapC = kernel_heapS;
    heap_head = NULL;
}

void *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    mem_block_t *curr = heap_head;

    while (curr)
    {
        if (curr->free && curr->size >= size)
        {
            curr->free = 0;
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }

    if (kernel_heapC + sizeof(mem_block_t) + size > kernel_heapE)
    {
        return NULL;
    }

    curr = (mem_block_t *)kernel_heapC;
    curr->size = size;
    curr->next = NULL;
    curr->free = 0;
    kernel_heapC += sizeof(mem_block_t) + size;

    if (!heap_head)
    {
        heap_head = curr;
    }
    else
    {
        mem_block_t *last = heap_head;
        while (last->next)
        {
            last = last->next;
        }
        last->next = curr;
    }

    return (void *)(curr + 1);
}

void *operator_new(size_t size)
{
    return malloc(size);
}

void operator_delete(void *ptr)
{
    free(ptr);
}

uintptr_t get_kernel_heap_start()
{
    return kernel_heapS;
}

uintptr_t get_kernel_heap_end()
{
    return kernel_heapE;
}

void free(void *ptr)
{
    if (!ptr)
        return;

    mem_block_t *block = ((mem_block_t *)ptr) - 1;
    block->free = 1;
}

void print_heap_blocks()
{
    puts("\n===== Heap Block Layout =====\n");

    mem_block_t *curr = heap_head;
    int block_number = 0;
    while (curr)
    {
        puts("Block ");
        print_dec(block_number++);
        puts(":\n");

        puts("  Address: ");
        print_hex((uint32_t)curr);
        puts("\n");

        puts("  Size (bytes): ");
        print_dec((uint32_t)curr->size);
        puts("\n");

        puts("  Status: ");
        if (curr->free)
        {
            puts("Free\n");
        }
        else
        {
            puts("Allocated\n");
        }

        curr = curr->next;
    }

    puts("===== End of Heap Layout =====\n");
}

void print_memory_layout()
{
    printf("============ Memory Layout ============\n");
    printf("Kernel Start Address      : 0x%08X\n", (uint32_t)&start);
    printf("Kernel End Address        : 0x%08X\n", (uint32_t)&end);
    printf("Kernel Heap Start Address : 0x%08X\n", get_kernel_heap_start());
    printf("Kernel Heap End Address   : 0x%08X\n", get_kernel_heap_end());
    printf("=======================================\n");
}
