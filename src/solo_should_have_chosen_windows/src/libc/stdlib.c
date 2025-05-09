#include "libc/stdlib.h"

void string_reverse_copy(char* from, char* to, int* size) {
    int j = 0;
    while ((*size) > 0) {
        to[j++] = from[--(*size)];
    }
    to[j] = '\0';
}

void itoa(int value, char* str) {
    char temp[32];
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value != 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (is_negative) {
        temp[i++] = '-';
    }

    string_reverse_copy(temp, str, &i);
}

void itoa_base(unsigned int value, char* str, int base) {
    char *digits = "0123456789ABCDEF";
    char temp[32];
    int i = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = digits[value % base];
        value /= base;
    }

    string_reverse_copy(temp, str, &i);
}