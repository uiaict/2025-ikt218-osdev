#include "libc/string.h"
#include "libc/stdarg.h"

// Function declarationsvoid 
reverse(char str[], int length);
char* itoa(int num, char* str, int base);

void putchar(char c, char* video_memory, size_t index) {
    video_memory[index * 2] = c;      // Character
    video_memory[index * 2 + 1] = 0x07; // Attribute byte (light gray on black)
}

void print_string(const char* str, char* video_memory) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        putchar(str[i], video_memory, i);
    }
}

void print_integer(int num, char* video_memory) {
    char buffer[12]; // Enough to hold INT_MIN
    itoa(num, buffer, 10); // Convert integer to string
    print_string(buffer, video_memory);
}

void printf(const char* format, ...) {
    char* video_memory = (char*)0xb8000; // Video memory address
    va_list args;
    va_start(args, format);
    
    size_t index = 0; // Index for video memory

    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%' && *(p + 1) == 'd') {
            int i = va_arg(args, int);
            print_integer(i, video_memory);
            char buffer[256] = {0};
            index += strlen(buffer); // Move index forward
            p++; // Skip the 'd'
        } else if (*p == '%' && *(p + 1) == 's') {
            char* s = va_arg(args, char*);
            print_string(s, video_memory);
            index += strlen(s); // Move index forward
            p++; // Skip the 's'
        } else {
            putchar(*p, video_memory, index++);
        }
    }

    va_end(args);
}