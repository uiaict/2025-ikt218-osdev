#include "../memory/memory.h"

#include "libc/stdint.h"
#include "libc/types.h"
#include "libc/stddef.h"

// Global operator new overloads
void* operator new(unsigned int size) {
    return malloc(size);
}

void* operator new[](unsigned int size) {
    return malloc(size);
}

// Global operator delete overloadsem
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Add sized-deallocation functions
void operator delete(void* ptr, unsigned int) noexcept {
    free(ptr);
}

void operator delete[](void* ptr, unsigned int) noexcept {
    free(ptr);
}

extern "C" {
    //functions are already declared in memory.h
}