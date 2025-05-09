#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/stdint.h"
#include "libc/stdlib.h"
#include "libc/stdbool.h"

// Compare two strings
int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Calculate string length
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
