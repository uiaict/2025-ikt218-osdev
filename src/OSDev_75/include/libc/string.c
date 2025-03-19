#include "stdint.h"
#include "stddef.h"
#include "string.h"

void* memset(void* dest, int value, size_t count) {
    unsigned char* ptr = (unsigned char*)dest;
    for (size_t i = 0; i < count; i++) {
        ptr[i] = (unsigned char)value;
    }
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* itoa(int value, char* str, int base) {
    // Handle special case of 0 explicitly
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    char* ptr = str;
    bool isNegative = false;
    unsigned int num;

    // Only handle negative if base == 10
    if (base == 10 && value < 0) {
        isNegative = true;
        num = (unsigned int)(-value);
    } else {
        num = (unsigned int)value;
    }

    // Convert the number to string (in reverse)
    int i = 0;
    while (num != 0) {
        int rem = num % base;
        ptr[i++] = (char)((rem > 9) ? (rem - 10 + 'a') : (rem + '0'));
        num /= base;
    }

    // Add minus sign if needed
    if (isNegative) {
        ptr[i++] = '-';
    }

    // Terminate string
    ptr[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = ptr[start];
        ptr[start] = ptr[end];
        ptr[end] = temp;
        start++;
        end--;
    }

    return str;
}
