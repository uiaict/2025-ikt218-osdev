#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "string.h"

void* memset(void* dest, int value, size_t count) {
    unsigned char* ptr = (unsigned char*)dest;
    for (size_t i = 0; i < count; i++) {
        ptr[i] = (unsigned char)value;
    }
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* itoa(int value, char* str, int base) {
    // Handle special case of 0 explicitly
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    char* ptr = str;
    bool isNegative = false;
    unsigned int num;

    // Only handle negative if base == 10
    if (base == 10 && value < 0) {
        isNegative = true;
        num = (unsigned int)(-value);
    } else {
        num = (unsigned int)value;
    }

    // Convert the number to string (in reverse)
    int i = 0;
    while (num != 0) {
        int rem = num % base;
        ptr[i++] = (char)((rem > 9) ? (rem - 10 + 'a') : (rem + '0'));
        num /= base;
    }

    // Add minus sign if needed
    if (isNegative) {
        ptr[i++] = '-';
    }

    // Terminate string
    ptr[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = ptr[start];
        ptr[start] = ptr[end];
        ptr[end] = temp;
        start++;
        end--;
    }

    return str;
}

// Copy string src to dest
char* strcpy(char* dest, const char* src) {
    size_t i = 0;
    while ((dest[i] = src[i]) != '\0') {
        i++;
    }
    return dest;
}

// Concatenate src to the end of dest
char* strcat(char* dest, const char* src) {
    char* ptr = dest + strlen(dest);
    while ((*ptr = *src) != '\0') {
        ptr++;
        src++;
    }
    return dest;
}

// Simple implementation of sprintf with support for %d
int sprintf(char* str, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    int written = 0;
    
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            // Regular character
            str[written++] = format[i];
        } else {
            // Format specifier
            i++; // Move past '%'
            
            if (format[i] == 'd') {
                // Integer
                int val = va_arg(args, int);
                char buffer[32];
                itoa(val, buffer, 10);
                
                // Copy the converted number
                for (int j = 0; buffer[j] != '\0'; j++) {
                    str[written++] = buffer[j];
                }
            } else if (format[i] == 's') {
                // String
                char* s = va_arg(args, char*);
                while (*s) {
                    str[written++] = *s++;
                }
            } else if (format[i] == '%') {
                // Literal %
                str[written++] = '%';
            }
            // Add more format specifiers as needed
        }
    }
    
    // Add null terminator
    str[written] = '\0';
    
    va_end(args);
    return written;
}