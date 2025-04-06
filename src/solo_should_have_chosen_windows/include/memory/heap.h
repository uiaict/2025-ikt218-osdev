#pragma once


#include "libc/stdint.h"

void heap_init (void* heap_mem_start, size_t heap_size);
void* malloc (size_t size);
void free (void* ptr);