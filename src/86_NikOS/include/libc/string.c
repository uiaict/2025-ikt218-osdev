#include "string.h"
#include <stddef.h>
#include <stdint.h>
#include "libc/stdbool.h"

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

int32_t strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

char* strchr(const char* str, int32_t c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* strtok(char* str, const char* delim) {
    static char* next = NULL;
    if (str) next = str;

    if (!next) return NULL;

    while (*next && strchr(delim, *next)) next++;
    if (*next == '\0') return NULL;

    char* start = next;

    while (*next && !strchr(delim, *next)) next++;

    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = NULL;
    }

    return start;
}

void* memset(void* dest, int32_t value, size_t count) {
    unsigned char* ptr = dest;
    while (count-- > 0)
        *ptr++ = (unsigned char)value;
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

bool strcontains(const char* str, const char delim) {
    while (*str) {
        if (*str == delim) {
            return true;
        }
        str++;
    }
    return false;
}