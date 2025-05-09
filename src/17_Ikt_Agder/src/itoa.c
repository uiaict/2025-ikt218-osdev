#include "libc/stdarg.h"
#include "libc/stdlib.h"
#include "libc/string.h"
#include "libc/stddef.h"

// Function to reverse a string
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void my_function() {
    char *buffer = malloc(256 * sizeof(char));
    if (!buffer) {
        return; // Check for allocation failure
    }

    // Use buffer for something, e.g., storing string or data

    free(buffer); // Free allocated memory after use
}

// Converts an integer to a string in a specified base
char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, as the following loop will not run
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers only for base 10
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    reverse(str, i); // Call reverse on the string after it's built

    return str;
}
