// paging.c

#include <stdint.h>
#include <kernel/memory/memory.h>

////////////////////////////////////////
// Page Directory and First Page Table
////////////////////////////////////////

// Page directory aligned to 4 KB
static uint32_t page_directory[1024] __attribute__((aligned(4096)));

// First page table (identity mapping)
static uint32_t first_table[1024] __attribute__((aligned(4096)));
