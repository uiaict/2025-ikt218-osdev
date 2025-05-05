#include "stdint.h"
#include "string.h"

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len]){
        len++;
    }
    return len;
}


void strrev(unsigned char s[])
{
    int i, j;
    unsigned char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}