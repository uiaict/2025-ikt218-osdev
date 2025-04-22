#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "libc/limits.h"
#include "libc/string.h"
#include "libc/stdlib.h"
#include "io.h"

int putchar(int ic) { 

    
    if (ic < 0 || ic > 255) {
        return EOF;
    } 

    
    char c = (char) ic; 

    switch (c) {
        case '\n': 
            cursorPos = (cursorPos / 160 + 1) * 160; 
            break;

        case '\r': 
            cursorPos = cursorPos / 160 * 160; 
            break;

        case '\t': 
            cursorPos = (cursorPos / 8 + 1) * 8; 
            break;

        case '\b': 
        
           
            if (cursorPos == 0) {
                break;
            }

            cursorPos -= 2; 
            break;

        default: 
            videoMemory[cursorPos] = c;
            videoMemory[cursorPos + 1] = currentTextColor; 
            videoMemory[cursorPos + 1] |= currentBackgroundColor << 4; 
            cursorPos += 2;  
    }

    

    
    if (cursorPos >= 160 * 25) { 
        scroll(); 
        cursorPos -= 160; 
    }
    
    setCursorPosition(cursorPos/2); 

    return ic;
}


bool print(const char* data, size_t length) {

  
	const unsigned char* bytes = (const unsigned char*) data; 

    
	for (size_t i = 0; i < length; i++) {
        if (putchar(bytes[i]) == EOF) {
            return false;
        }
    }

	return true;
} 


// Printf implementation
int printf(const char* __restrict__ format, ...) {

  
    va_list parameters;
    va_start(parameters, format);

    int written = 0;


    while (*format != '\0') {
        if (*format != '%') {
            putchar(*format);
            format++;
            written++;
            continue;
        }
        format++;

        if (*format == '%') {
            putchar(*format);
            format++;
            written++;
            continue;
        }

        if (*format == 'c') {
            char c = (char) va_arg(parameters, int); 
            putchar(c);
            format++;
            written++;
            continue;
        }

        if (*format == 's') {
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            print(str, len);
            format++;
            written += len;
            continue;
        }

        if (*format == 'd' || *format == 'i') {
            int num = va_arg(parameters, int);
            char str[32];
            itoa(num, str, 10);
            size_t len = strlen(str);
            print(str, len);
            format++;
            written += len;
            continue;
        }

        if (*format == 'u') {
            unsigned int num = va_arg(parameters, unsigned int);
            char str[32];
            utoa(num, str, 10);
            size_t len = strlen(str);
            print(str, len);
            format++;
            written += len;
            continue;
        }

        if (*format == 'f') {
            float num = va_arg(parameters, double);
            char str[32];
            ftoa(num, str, 6);
            size_t len = strlen(str);
            print(str, len);
            format++;
            written += len;
            continue;
        }

        if (*format == '.') {
            format++;
            int precision = 0;
            while (*format >= '0' && *format <= '9') {
                precision = precision * 10 + (*format - '0');
                format++;
            }
            if (*format == 'f') {
                float num = va_arg(parameters, double);
                char str[32];
                ftoa(num, str, precision);
                size_t len = strlen(str);
                print(str, len);
                format++;
                written += len;
                continue;
            }
        }


    
    }
    va_end(parameters);
    return written;
}

char getchar() {
    char c = 0;
    while (c == 0) {
        c = inb(0x60);
    }
    return c;
}