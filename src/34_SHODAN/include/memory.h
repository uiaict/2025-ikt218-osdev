#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void init_kernel_memory(uint32_t* kernel_end);
void init_paging();  
void* malloc(uint32_t size);
void free(void* ptr);
void print_memory_layout();

#endif
