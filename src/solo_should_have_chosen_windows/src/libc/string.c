#include "libc/string.h"

void *memset(void *dest, int val, size_t len) {
    uint8_t *ptr = (uint8_t *)dest;
    for (size_t i = 0; i < len; i++) {
        ptr[i] = (uint8_t) val;
    }
    return dest;
}