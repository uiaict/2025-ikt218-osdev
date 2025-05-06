#ifndef PAGING_H
#define PAGING_H

#include "libc/stdint.h"

// Sets up page tables and initial mappings
void init_paging();

// Maps one 4KB page from virtual to physical address
void paging_map(uint32_t virt_addr, uint32_t phys_addr);

// Enables paging by setting CR0 bit
void paging_enable();

#endif
