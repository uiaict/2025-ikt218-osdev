#include "libc/stdlib.h"

void ftoa(float value, char* buffer, int precision) {
    if (value < 0) {
        *buffer++ = '-';
        value = -value;
    }

    int int_part = (int)value;
    float fraction = value - int_part;

    // Convert integer part
    char int_buf[20];
    int i = 0;
    if (int_part == 0) {
        int_buf[i++] = '0';
    } else {
        while (int_part > 0) {
            int_buf[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }
    while (i > 0) {
        *buffer++ = int_buf[--i];
    }

    *buffer++ = '.';

    // Convert fractional part
    for (int j = 0; j < precision; j++) {
        fraction *= 10.0f;
        int digit = (int)fraction;
        *buffer++ = '0' + digit;
        fraction -= digit;
    }

    *buffer = '\0';
}