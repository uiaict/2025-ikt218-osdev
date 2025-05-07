// paging.c   
#include <stdint.h>
#include "kmem.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_table[1024]   __attribute__((aligned(4096)));

void init_paging(void)
{
    
    for (int i = 0; i < 1024; ++i)
        first_table[i] = (i * 0x1000) | 3;   

    page_directory[0] = ((uint32_t)first_table) | 3;
    for (int i = 1; i < 1024; ++i) page_directory[i] = 0;

    asm volatile ("mov %0, %%cr3" :: "r"(page_directory));
    uint32_t cr0; asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; 
    asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}
