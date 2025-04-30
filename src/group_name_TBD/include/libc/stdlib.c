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
 
    return s;
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
 
    return s;
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