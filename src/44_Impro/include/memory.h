#include <libc/stdint.h>



void* malloc(uint32_t size);
void free(void* ptr);
void init_kernel_memory(uint32_t* end);
void init_paging();
void print_memory_layout();
void init_pit();