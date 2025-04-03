#include "libc/string.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* strcat(char* dest, const char* src) {
    char* dest_ptr = dest + strlen(dest);
    while (*src) {
        *dest_ptr++ = *src++;
    }
    *dest_ptr = '\0';
    return dest;
}