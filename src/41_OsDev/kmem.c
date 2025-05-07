// kmem.c  
#include "kmem.h"
#include <stddef.h>


static uint32_t heap_start;
static uint32_t heap_end;
static uint32_t placement;

void init_kernel_memory(uint32_t* kernel_end)
{
    heap_start = (uint32_t)kernel_end;
    if (heap_start & 0xFFF)        
        heap_start = (heap_start & 0xFFFFF000) + 0x1000;

    placement = heap_start;
    heap_end  = heap_start + 8 * 1024 * 1024;   
}

void* kmalloc(uint32_t bytes, int align)
{
    if (align && (placement & 0xFFF))
        placement = (placement & 0xFFFFF000) + 0x1000;

    uint32_t addr = placement;
    placement += bytes;
    if (placement > heap_end)
        return NULL;              

    return (void*)addr;
}

void kfree(void* ptr)
{
    (void)ptr; 
}
