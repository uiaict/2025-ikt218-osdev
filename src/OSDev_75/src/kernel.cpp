#include "../memory/memory.h"

#include "libc/stdint.h"
#include "libc/types.h"
#include "libc/stddef.h"

void* operator new(unsigned int size) {
    return malloc(size);
}

void* operator new[](unsigned int size) {
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, unsigned int) noexcept {
    free(ptr);
}

void operator delete[](void* ptr, unsigned int) noexcept {
    free(ptr);
}

extern "C" {
}