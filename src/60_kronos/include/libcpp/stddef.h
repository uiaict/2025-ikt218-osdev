#pragma once

#define NULL ((void*)0)

typedef unsigned long size_t;


extern "C" {
    #include "memory/memory.h"
}
extern "C" void* malloc(size_t size);
extern "C" void free(void* ptr);