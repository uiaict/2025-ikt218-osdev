#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>

#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

void init_kernel_memory(void* kernel_end);

void* malloc(size_t size);
uintptr_t get_kernel_heap_start();
uintptr_t get_kernel_heap_end();

#endif // KERNEL_MEMORY_H