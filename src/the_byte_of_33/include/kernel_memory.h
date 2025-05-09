#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>

#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t size);
void free(void* ptr);

void* operator_new(size_t size);
void operator_delete(void* ptr);

void print_heap_blocks(void);

#ifdef __cplusplus
}
#endif

void init_kernel_memory(void* kernel_end);
uintptr_t get_kernel_heap_start();
uintptr_t get_kernel_heap_end();


#endif