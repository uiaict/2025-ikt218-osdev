#ifndef UTIL
#define UTIL

#include "libc/stdint.h"
#include "util.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

void reverse(char* str, int length) {
    int ptrLeft = 0;
    int ptrRight = length-1;
    while(ptrLeft < ptrRight){
        char temp = str[ptrLeft];
        str[ptrLeft] = str[ptrRight];
        str[ptrRight] = temp;
        ptrLeft++;
        ptrRight--;
    }
}

// https://www.geeksforgeeks.org/implement-itoa/
char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    if(num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if(num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    while(num != 0) {
        int rem = num % base;
        str[i++] = (rem>9) ? (rem-10) + 'a' : rem + '0';
        num = num / base;
    }

    if(isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverse(str,i);

    return str;

}

// https://www.geeksforgeeks.org/convert-floating-point-number-string/?ref=ml_lbp
void ftoa(float num, char *str, int afterpoint) {
    int ipart = (int) num;
    float fpart = num - (float) ipart;

    itoa(ipart, str, 10);
    
    int i = strlen(str);

    if (afterpoint != 0) {
        str[i] = '.';
        i++;
        for (int j = 0; j < afterpoint; j++) {
            fpart = fpart * 10;
            int frac = (int)fpart;
            str[i++] = frac + '0';
            fpart -= frac;
        }
    }
    str[i] = '\0';
}

void memset(void* dest, char val, uint32_t count) {
    char* temp = (char*) dest;
    for(; count != 0; count--) {
        *temp++ = val;
    }
}

void outPortB(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

char inPortB(uint16_t port) {
    char rv;
    asm volatile ("inb %1, %0" : "=a"(rv) : "dN"(port));
}

#endif