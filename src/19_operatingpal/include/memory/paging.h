#ifndef PAGING_H
#define PAGING_H

#include "libc/stdint.h"

void init_paging(); // Setter opp paging og aktiverer det
void paging_map(uint32_t virt_addr, uint32_t phys_addr); // Mapper én side (4KB)
void paging_enable(); // Aktiverer paging i CR0

#endif
