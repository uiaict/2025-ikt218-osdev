#include <libc/string.h>
#include <libc/stddef.h>

void *memset(void *dest, int val, size_t count) {
    unsigned char *p = dest;
    while (count--) *p++ = (unsigned char)val;
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}