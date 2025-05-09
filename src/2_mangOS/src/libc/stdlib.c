#include "libc/stdlib.h"

char* itoa(int value, char* str, int base)
{
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
        ptr1++;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value % base];
    } while (value);

    *ptr-- = '\0';

    // Reverse the string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}
