#include "libc/stdlib.h"
#include "libc/string.h"
#include "libc/math.h"

// Implementation of the itoa function adapted from https://www.geeksforgeeks.org/implement-itoa/
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; 
 
    strrev(str, i);
 
    return str;
}

char* utoa(unsigned int num, char* str, int base)
{
    unsigned int i = 0;
 
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    str[i] = '\0'; 
 
    strrev(str, i);
 
    return str;
}



// Implementation of the ftoa function. Adapted from https://www.geeksforgeeks.org/convert-floating-point-number-string/
void ftoa(float n, char* res, int afterpoint) {
    int intPart = (int)n;

    float floatPart = n - (float)intPart;

    itoa(intPart, res, 10);  

    int i = 0;
    while (res[i] != '\0') {
        i++;
    }

    if (afterpoint != 0) {
        res[i] = '.';
        int power = 1;
        for (int j = 0; j < afterpoint; j++) {
            power *= 10;
        }

        floatPart = floatPart * power;

        itoa((int)floatPart, res + i + 1, 10);  
    }
}


// Implementation of the atoi function from https://www.geeksforgeeks.org/write-your-own-atoi/
int atoi(char *str)
{
    int res = 0;
    int sign = 1;
    int i = 0;
 

    if (str[0] == '-') {
        sign = -1;
 
        i++;
    }
 
    for (; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
 
    return sign * res;
}