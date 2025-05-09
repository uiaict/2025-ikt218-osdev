#include <stdint.h>
#include <stddef.h>
#include "libc/string.h"
#include "libc/conv.h"

void itoa(int32_t value, char* str) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int32_t tmp_value;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    if (value < 0) {
        *ptr++ = '-';
        value = -value;
        ptr1 = ptr;
    }

    while (value) {
        tmp_value = value;
        value /= 10;
        *ptr++ = '0' + (tmp_value - value * 10);
    }

    *ptr = '\0';

    while (ptr1 < --ptr) {
        tmp_char = *ptr;
        *ptr = *ptr1;
        *ptr1 = tmp_char;
        ptr1++;
    }
}

void uitoa(uint32_t value, char* str) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    while (value) {
        *ptr++ = '0' + (value % 10);
        value /= 10;
    }

    *ptr = '\0';

    while (ptr1 < --ptr) {
        tmp_char = *ptr;
        *ptr = *ptr1;
        *ptr1 = tmp_char;
        ptr1++;
    }
}