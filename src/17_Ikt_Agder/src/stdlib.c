#include "libc/stdlib.h"
#include "libc/stddef.h"

void *malloc(size_t size) {
    // Stub implementation for OS dev (always returns NULL for now)
    return NULL;
}

void free(void *ptr) {
    // Stub: No-op
}

void *calloc(size_t num, size_t size) {
    return NULL;  // Stub implementation
}

void *realloc(void *ptr, size_t new_size) {
    return NULL;  // Stub implementation
}

int atoi(const char *str) {
    int result = 0;
    while (*str) {
        if (*str < '0' || *str > '9') break;
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

void exit(int status) {
    while (1) {}  // Infinite loop (since we have no OS shutdown logic)
}
