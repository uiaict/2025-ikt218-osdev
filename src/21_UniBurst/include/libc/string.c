#include <libc/string.h>
#include "libc/stdint.h"
#include "libc/stddef.h"

size_t strlen(char *str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// Implementation of strrev function adapted from https://www.geeksforgeeks.org/implement-itoa/
void strrev(char str[], int length)
{
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


int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}


char *strchr(char *str, int c) 
{
    while (*str != '\0') {
        if (*str == c) 
            return str;
        str++;
    }

    if (c == '\0')
        return str;
    return NULL;
}

