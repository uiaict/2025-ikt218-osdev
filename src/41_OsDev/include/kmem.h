// kmem.h
#pragma once
#include <stdint.h>


void*  kmalloc(uint32_t bytes, int align);
void   kfree(void*);

#define malloc(sz)           kmalloc((sz), 0)
#define malloc_aligned(sz)   kmalloc((sz), 1)
#define free(ptr)            kfree(ptr)
