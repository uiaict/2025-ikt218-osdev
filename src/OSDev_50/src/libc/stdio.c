#include "libc/system.h"
#include "libc/stdarg.h"
#include "libc/stdio.h"
#include "libc/stdint.h"
#include "libc/limits.h"
#include "libc/string.h"


extern void monitor_put(char c);

int putchar(int ic) {
    char c = (char) ic;
    monitor_put(c);
	return ic;
}

bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}


int printf(const char* __restrict__ format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;
    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount)
                return -1;
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        
        format++; 
        if (*format == 'c') {

            format++;
            char c = (char)va_arg(parameters, int);
            if (!maxrem) return -1;
            if (!print(&c, 1)) return -1;
            written++;
        }
        else if (*format == 's') {

            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) return -1;
            if (!print(str, len)) return -1;
            written += len;
        }
        else if (*format == 'd') {

            format++;
            int num = va_arg(parameters, int);
            char buffer[20];
            int i = 0;
            unsigned int unum;

            if (num < 0) {
    
                if (!print("-", 1)) return -1;
                written++;

                unum = (unsigned)(-(num + 1)) + 1;
            } else {
                unum = (unsigned)num;
            }

            if (unum == 0) {
                buffer[i++] = '0';
            } else {
                while (unum != 0) {
                    buffer[i++] = '0' + (unum % 10);
                    unum /= 10;
                }
            }
    
            while (i > 0) {
                if (!print(&buffer[--i], 1)) return -1;
                written++;
            }
        }
        else if (*format == 'u') {
       
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            char buffer[20];
            int i = 0;

            if (num == 0) {
                buffer[i++] = '0';
            } else {
                while (num != 0) {
                    buffer[i++] = '0' + (num % 10);
                    num /= 10;
                }
            }
            while (i > 0) {
                if (!print(&buffer[--i], 1)) return -1;
                written++;
            }
        }
        else if (*format == 'x') {
        
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            char buffer[20];
            int i = 0;
            if (num == 0) {
                buffer[i++] = '0';
            } else {
                while (num != 0) {
                    int rem = num % 16;
                    buffer[i++] = (rem < 10) ? ('0' + rem) : ('a' + rem - 10);
                    num /= 16;
                }
            }
            while (i > 0) {
                if (!print(&buffer[--i], 1)) return -1;
                written++;
            }
        }
        else {
            
            const char* start = format - 1;
            size_t len = strlen(start);
            if (maxrem < len) return -1;
            if (!print(start, len)) return -1;
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}