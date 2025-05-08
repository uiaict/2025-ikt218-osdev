// paging.c   
#include <stdint.h>
#include <kernel/memory.h>

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_table[1024]   __attribute__((aligned(4096)));
