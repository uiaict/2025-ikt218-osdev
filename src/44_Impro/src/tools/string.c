#include "libc/string.h"


size_t strlen(const char* str){
    size_t len = 0;
    while(str[len]){
        len++;
    }
    return len;
}


void int_to_str(int value, char* buffer) {
    char temp[16];
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative) {
        temp[i++] = '-';
    }

    int j = 0;
    while (i--) {
        buffer[j++] = temp[i];
    }
    buffer[j] = '\0';
}