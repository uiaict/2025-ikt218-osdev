#include "songPlayer.h"
extern "C" {
    #include "libc/stdio.h"
    #include "memory.h"
    #include "libc/stdint.h"
    #include "song_data1.h"
    #include "song_data2.h"
    #include "matrix_rain.h"
}

// Operator new/delete
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Optional sized-delete overloads
void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    free(ptr);
}

// C++ entry point
extern "C" int kernel_main() {
    printf("\n");
    printf("Inside C++ kernel_main()\n");

    int* x = new int(123);
    printf("Value from new: %d\n", *x);
    delete x;

    // Play a long
    printf("Starting playing...\n");
    play_song(&temple_song);
    printf("Song ended.\n");

    // Matrix effect
    run_matrix_rain();

    while (true) {}
}
