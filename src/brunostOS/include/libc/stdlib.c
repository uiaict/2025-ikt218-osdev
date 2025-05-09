#include "stdlib.h"
#include "libc/string.h"
#include "stdbool.h"

void itoa(long int n, char s[]){

    int i = 0;
    bool isNegative = false;
 

    if (n == 0) {
        s[i] = '0';
        i++;
        s[i] = '\0';
        return s;
    }
 
    if (n < 0){
        isNegative = true;
        n = -n;
    }
 
    // Process individual digits
    while (n != 0) {
        int rem = n % 10;
        s[i] = rem + '0';  // Convert digit to character
        i++;
        n = n / 10;
    }
 
    // If number is negative, append '-'
    if (isNegative){
        s[i] = '-';
        i++;
    }
 
    // Reverse the string
    strrev(s);
    s[i] = '\0'; // Append string terminator
 
}

void utoa(unsigned long int n, char s[]){

    int i = 0; 

    if (n == 0) {
        s[i] = '0';
        i++;
        s[i] = '\0';
        return s;
    }
 
    // Process individual digits
    while (n != 0) {
        int rem = n % 10;
        s[i] = rem + '0';  // Convert digit to character
        i++;
        n = n / 10;
    }
 
    // Reverse the string
    strrev(s);
    s[i] = '\0'; // Append string terminator
 
}

void ftoa(double n, char s[], int precision){

    int i_part = (int)n;
    double f_part = n-i_part;

    itoa(i_part, s);

    int i = 0;
    while (s[i] != '\0') {
        i++;
    }

    if(precision != 0){
        s[i] = '.';
    }

    for (int j = 0; j < precision; j++) {
        f_part *= 10;
    }
    
    utoa((unsigned long int)f_part, s+i+1);
    int x = 0;
}


void xtoa(unsigned long int n, char s[]){
    unsigned char hex_digits[] = "0123456789ABCDEF";

    int i = 0;
    while(n != 0){
        s[i] = hex_digits[n%16];
        n = n/16;
        i++;
    }
    strrev(s);
    s[i] = '\0';
}


void atoi(const char s[], long int *n){

    bool is_negative = false;
    int i = 0;

    if (s[i] == '-'){
        is_negative = true;
        i++;
    }
    
    while (i < strlen(s)){

        *n *= 10;
        int num = ((int)s[i] - (int)('0'));
        *n += num;

        i++;
    }

    if (is_negative){
        *n = -(*n);
    }
}

void atou(const char s[], unsigned long int *n){

    int i = 0;
    while (i < strlen(s)){

        *n *= 10;
        unsigned int num = ((unsigned int)s[i] - (unsigned int)('0'));
        *n += num;

        i++;
    }
}

void atof(const char s[], double *n){

    bool is_negative = false;
    int i = 0;

    if (s[i] == '-'){
        is_negative = true;
        i++;
    }
    
    while (i < strlen(s)){
        if (s[i] == '.' || s[i] == ','){ // check for ',' doen't hurt
            i++;
            break;
        }

        *n *= 10;
        int num = ((int)s[i] - (int)('0'));
        *n += (double)num;
        i++;
    }

    double decimals = 0;
    int stop = i;
    i = strlen(s)-1;

    while (i >= stop){
    
        int num = ((int)s[i] - (int)('0'));
        decimals += (double)num;
        i--;
        decimals /= 10;
    }
    *n += decimals;

    if (is_negative){
        *n = -(*n);
    }
}
