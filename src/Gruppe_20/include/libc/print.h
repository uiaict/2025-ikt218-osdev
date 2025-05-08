#ifndef PRINT_H
#define PRINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libc/stdint.h>
#include <libc/stdarg.h>
void print_string(const char* str);

// Function to print a single character to the screen
void print_char(char c);

// Function to print an integer to the screen
void print_int(int value);

// Function to handle variable arguments and print formatted strings to the screen
void vprintf(const char* format, va_list args);

// Function to print formatted strings to the screen (variadic function)
void printf(const char* format, ...);


#ifdef __cplusplus
}  // <<< CLOSE the extern "C"
#endif

#endif // PRINT_H
