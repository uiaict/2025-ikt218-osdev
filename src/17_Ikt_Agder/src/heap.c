#include "libc/stddef.h"
#include "libc/stdint.h"
#include "libc/stdlib.h"
#include "libc/stdio.h"

#define HEAP_START 0x100000  // 1MB heap start
#define HEAP_SIZE  0x100000  // 1MB heap size

static uint8_t heap[HEAP_SIZE];
static size_t heap_index = 0;

void* malloc(size_t size) {
    if (heap_index + size >= HEAP_SIZE) {
        printf("Out of memory!\n");
        return NULL;
    }
    void* ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

void free(void* ptr) {
    // Simple bump allocator: doesn't free memory
}

//#define HEAP_SIZE 1024 * 1024  // 1MB heap                                          vet ikke om alt nedenfor her trengs lenger ......
//static unsigned char heap[HEAP_SIZE];
//static size_t heap_index = 0;

//void *malloc(size_t size) {
//    if (heap_index + size > HEAP_SIZE) return NULL;  // No space left
//    void *ptr = &heap[heap_index];
//    heap_index += size;
//    return ptr;
//}

//void free(void *ptr) {
    // Simple allocator: No actual freeing
//}
