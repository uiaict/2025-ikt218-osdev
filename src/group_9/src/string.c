#include "string.h"
#include <stddef.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* itoa(int value, char* buffer, int base) {
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return buffer;
    }

    char* ptr = buffer;
    int abs_value = value < 0 ? -value : value;
    int remainder;

    do {
        remainder = abs_value % base;
        *ptr++ = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
        abs_value /= base;
    } while (abs_value > 0);

    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }

    *ptr = '\0';

    // Stringi ters çevir
    char* start = buffer;
    char* end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }

    return buffer;
}

void* memmove(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;

    if (d == s) {
        return d;
    }

    if (s < d && s + n > d) {
        // Overlapping, arkadan başla
        s += n;
        d += n;
        while (n--) {
            *--d = *--s;
        }
    } else {
        // Overlapping yok veya önden kopyalama güvenli
        while (n--) {
            *d++ = *s++;
        }
    }

    return dest;
}

void* memset(void* dest, int val, size_t len) {
    unsigned char* ptr = dest;
    while (len-- > 0)
        *ptr++ = (unsigned char)val;
    return dest;
}

