#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <libc/stdarg.h> 

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag first;
};

// Video memory base address
static char* video_memory = (char*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

// Write a string to the terminal
void terminal_write(const char* str) {
    while (*str) {
        // Handle newline
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
            str++;
            continue;
        }

        // Check if we need to wrap to next line
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }

        // Calculate the offset in video memory
        int offset = (cursor_y * 80 + cursor_x) * 2;
        
        // Write character and color attribute
        video_memory[offset] = *str;
        video_memory[offset + 1] = 0x0F;  // White text on black background

        str++;
        cursor_x++;
    }
}

static void itoa(int value, char* str, int base) {
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    
    // Handle 0 explicitly
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    int sign = 0;
    if (value < 0 && base == 10) {
        sign = 1;
        value = -value;
    }
    
    // Process digits in reverse
    int i = 0;
    while (value != 0) {
        str[i++] = digits[value % base];
        value /= base;
    }
    
    // Add negative sign if needed
    if (sign) {
        str[i++] = '-';
    }
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    
    // Null terminate
    str[i] = '\0';
}

// NEW: Simple implementation of printf
void terminal_printf(const char* format, ...) {
    char buffer[256];
    char num_buffer[32];
    char* buf_ptr = buffer;
    
    // Start variadic argument processing
    va_list args;
    va_start(args, format);
    
    // Process the format string
    while (*format != '\0') {
        if (*format != '%') {
            // Normal character - copy to buffer
            *buf_ptr++ = *format++;
            continue;
        }
        
        // Handle format specifier
        format++; // Skip the '%'
        
        switch (*format) {
            case 's': {
                // String
                char* str = va_arg(args, char*);
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'd':
            case 'i': {
                // Integer
                int value = va_arg(args, int);
                itoa(value, num_buffer, 10);
                char* str = num_buffer;
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'x': {
                // Hexadecimal
                unsigned int value = va_arg(args, unsigned int);
                itoa(value, num_buffer, 16);
                char* str = num_buffer;
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'c': {
                // Character
                *buf_ptr++ = (char)va_arg(args, int);
                break;
            }
            case '%': {
                // Literal '%'
                *buf_ptr++ = '%';
                break;
            }
            default: {
                // Unknown format specifier - just output it
                *buf_ptr++ = '%';
                *buf_ptr++ = *format;
                break;
            }
        }
        
        format++;
    }
    
    // Null terminate the buffer and print it
    *buf_ptr = '\0';
    terminal_write(buffer);
    
    // End variadic argument processing
    va_end(args);
}

extern void init_gdt(void);
int main(uint32_t magic, void* mb_info) {
    //Initialize the GDT
    init_gdt();
     // Write "Hello World"
  terminal_printf("Hello kernel with printf\n");  // Use printf for first message too
  terminal_printf("GDT has been initialized with NULL, CODE, and DATA segments.\n");
  
    // Hang the kernel
    while(1) {}
    
    return 0;
}