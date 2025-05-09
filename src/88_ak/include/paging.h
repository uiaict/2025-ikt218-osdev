#ifndef PAGING_H
#define PAGING_H

#include "libc/system.h"
#include "printf.h"

#define NUM_PAGES 1024   // hver page table og page directory har 1024 entries
#define PAGE_SIZE 0x1000 // 4kb per side

void init_paging();
void enable_paging();
void map_virt_to_phys(uint32_t virtualAddr, uint32_t physicalAddr);

#endif