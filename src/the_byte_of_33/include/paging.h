#ifndef PAGING_H
#define PAGING_H

#include <libc/stdint.h>

void init_paging();
void load_page_directory(uint32_t page_directory_address);
void enable_paging();

#endif
