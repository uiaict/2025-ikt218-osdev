#include "libc/string.h"


int strcmp(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++;
        b++;
    }
    return *a - *b;
}

// strcpy function
int strcpy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    return i; // Return the length of the copied string
}

//strlen function
int strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


char *strncpy(char *dest, const char *src, int n) {
    int i;

    // Copy characters from src to dest until n is reached or src ends
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // If src is shorter than n, pad the rest of dest with '\0'
    for ( ; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}

// string startswith function
int startsWith(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return 1; // not a match
        }
    }
    return 0; // match
}