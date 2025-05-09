// cpp_memory.cpp - C++ memory operators that call our C malloc/free

#include <libc/stddef.h>

////////////////////////////////////////
// C Declarations (no name mangling)
////////////////////////////////////////

extern "C" {
    void* malloc(size_t size);
    void  free(void* ptr);
}

////////////////////////////////////////
// C++ New/Delete Operators
////////////////////////////////////////

// Allocate memory using C malloc
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

// Free memory using C free
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Sized delete (C++14)
void operator delete(void* ptr, size_t) noexcept {
    free(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
    free(ptr);
}

////////////////////////////////////////
// Exception Support Stub
////////////////////////////////////////

// Dummy personality function required by some C++ runtimes
extern "C" void __gxx_personality_v0() {
    while (1);  // Trap forever — no exceptions supported
}
