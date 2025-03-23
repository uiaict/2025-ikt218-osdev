#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stdlib.h"

#include "libc/stddef.h"
#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define GREEN_ON_BLACK 0x02

// Used to store the current cursor position
static uint16_t cursor_position = 0;

// Function to print a single character at the current cursor position
static void print_char(char c) {
    // Points to the start of video memory text buffer
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;

    // Handle newline manually
    if (c == '\n') {
        cursor_position += SCREEN_WIDTH - (cursor_position % SCREEN_WIDTH);
    } else {
        video_memory[cursor_position] = (GREEN_ON_BLACK << 8) | c;
        cursor_position++;
    }
    
    move_cursor(cursor_position);
}

// Print function to print a string
static void print_string(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}


// Print function to print a formatted string
void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int i = 0;
    while (format[i] != '\0') {

        if (format[i] == '%' && format[i+1] != '\0') {
            switch (format[++i])
            {
            case 's': {
                char *str = va_arg(args, char*);
                print_string(str ? str : "(null)");
                break;
            }
            case 'c': {
                char c = (char) va_arg(args, int);
                print_char(c);
                break;
            }
            case 'd': {
                int value = va_arg(args, int);
                char str[12];
                itoa(value, str);
                print_string(str);
                break;
            } 
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                char str[12];
                itoa_base(value, str, 16);
                print_string("0x");
                print_string(str);
                break;
            }
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                char str[12];
                itoa_base(value, str, 10);
                print_string(str);
                break;
            }
            case 'p': {
                void* ptr = va_arg(args, void*);
                uintptr_t addr = (uintptr_t)ptr;
            
                char str[9];  // 8 hex digits max for 32-bit + null
                itoa_base(addr, str, 16);
            
                // Print prefix
                print_string("0x");
            
                // Pad with leading zeros (8 - actual length)
                int len = 0;
                while (str[len] != '\0') len++;
            
                for (int j = 0; j < 8 - len; j++) {
                    print_char('0');
                }
            
                print_string(str);
                break;
            }
            case '%':
                print_char('%');
                break;
            default:
                print_string("(unknown format descriptor)");
                va_arg(args, void*);
                break;
            }
        }
        else {
            print_char(format[i]);
        }
        i++;
    }

    va_end(args);
}
