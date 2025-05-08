#include "common/itoa.h"


// Converts an unsigned int to a string (in any base up to 16)
void itoa(unsigned int value, char* buffer, int base) {
    const char* digits = "0123456789abcdef";
    char temp[32];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    while (value > 0) {
        temp[i++] = digits[value % base];
        value /= base;
    }

    // Reverse the string into the output buffer
    for (int j = 0; j < i; j++) {
        buffer[j] = temp[i - j - 1];
    }
    buffer[i] = '\0';
}
