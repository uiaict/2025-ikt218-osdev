#include "libc/stdio.h"
#include "monitor.h"
#include "libc/stdarg.h"
#include "system.h"
#include "libc/limits.h"
#include "libc/errno.h"
#include "libc/string.h"


// Prints a char and returns -1 for EOF
int putchar(int ic)
{
    char c = (char) ic;
    monitorPut(c);
    return ic;
}

// Printes a string 
bool print(const char* data, size_t length)
{
    const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

// Printes a string and suports %c, %s, %d, %x, and %%
int printf(const char* __restrict__ format, ...) {
	va_list parameters;
	va_start(parameters, format);	// Initialize the variable argument list
 
	int written = 0;	// Total amount of characters written
 

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		// Handles sting and %%
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;	// Skip the first % in %%
			
			// Counts how many characters to print before the next format specifier
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				errno = EOVERFLOW;
				return -1;
			}
			if (!print(format, amount))		// Prints the characters counted above
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 

		// Begin parsing a format specifier
		const char* format_begun_at = format++;
 
		// Handles %c, a character
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				errno = EOVERFLOW;
				return -1;
			}
			if (!print(&c, sizeof(c)))	// Print a single character
				return -1;
			written++;

		// Handles %s, null-terminated string
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				errno = EOVERFLOW;
				return -1;
			}
			if (!print(str, len))	// Print the string
				return -1;
			written += len;

		// Handle %d, signed decimal integer
		} else if (*format == 'd') {
            format++;
            int num = va_arg(parameters, int);
            char buffer[20];	// Enough to hold -2,147,483,648
            int i = 0;

            if (num == 0) {
                buffer[i++] = '0';
            } else if (num < 0) {
                buffer[i++] = '-';
                num = -num;
            }

			// Convert number to string in reverse order
            while (num != 0) {
                buffer[i++] = num % 10 + '0';
                num /= 10;
            }

			// Prints the characters in reverse
            while (i > 0) {
                if (maxrem < 1) {
                    errno = EOVERFLOW;
                    return -1;
                }
                if (!print(&buffer[--i], 1))
                    return -1;
                written++;
            }
		
		// Handle %x, unsigned hexadecimal
        } else if (*format == 'x') {
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            char buffer[20];
            int i = 0;
            if (num == 0) {
                buffer[i++] = '0';
            }
            while (num != 0) {
                int rem = num % 16;
                if (rem < 10) {
                    buffer[i++] = rem + '0';		// Handles numbers
                } else {
                    buffer[i++] = rem - 10 + 'a';	// Handles letters
                }
                num /= 16;
            }

			// Convert to hex in reverse order
            while (i > 0) {
                if (maxrem    < 1) {
                errno = EOVERFLOW;
                return -1;
            }
			
			// Prints hex sting in reverse
            if (!print(&buffer[--i], 1))
                return -1;
            written++;
			}

		// Handles unknown specefiers and just prints
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				errno = EOVERFLOW;
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);		// Clean up argument list
	return written;
}