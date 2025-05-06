#include "libc/string.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

// Returns length of a string
size_t strlen(char *str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// Finds first occurrence of character in string
char *strchr(char *str, int c) {
    while (*str != '\0') {
        if (*str == c) 
            return str;
        str++;
    }
    if (c == '\0')
        return str;
    return NULL;
}

// Reverses a string in-place
void strrev(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

// Compares two strings
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Sets memory to a value
void* memset(void* dest, int val, size_t len) {
    unsigned char* ptr = dest;
    while (len-- > 0)
        *ptr++ = (unsigned char)val;
    return dest;
}
