#include "libc/string.h"
#include "libc/stddef.h"

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}