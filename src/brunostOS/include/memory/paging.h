#ifndef PAGING_H
#define PAGING_H

#include "libc/stdint.h"

/* Function declarations for paging operations */
void init_paging(); /* Initializes paging */
void paging_map_virtual_to_phys(uint32_t, uint32_t); /* Maps a virtual address to a physical address */

#endif // PAGING_H