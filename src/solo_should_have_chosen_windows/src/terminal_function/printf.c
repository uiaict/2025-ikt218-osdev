#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stdlib.h"

#include "libc/stddef.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/string.h"

#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define GREEN_ON_BLACK 0x02

// Used to store the current cursor position
static uint16_t cursor_position = 0;
bool old_logs = false;

static void scroll_or_reset() {
    cursor_position = 0;
    move_cursor(cursor_position);
    old_logs = true;  
}

// Function to print a single character at the current cursor position
static void print_char(char c) {
    // Check if the cursor position is at the end of the screen
    if (cursor_position >= SCREEN_WIDTH * SCREEN_HEIGHT)
        scroll_or_reset();
    
        // Points to the start of video memory text buffer
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;

    if (cursor_position % SCREEN_WIDTH == 0 && old_logs) {
        int temp_pos = cursor_position;
        // Clear two lines
        for (int i = 0; i < SCREEN_WIDTH * 2; i++) {
            video_memory[temp_pos + i] = (GREEN_ON_BLACK << 8) | ' ';
        }

        temp_pos += SCREEN_WIDTH * 2;
        const char *old_marker = "---- OLD LOGS BELOW ----";
        size_t old_marker_len = strlen(old_marker);
        for (size_t i = 0; i < SCREEN_WIDTH; i++) {
            if (i < old_marker_len) {
                video_memory[temp_pos + i] = (GREEN_ON_BLACK << 8) | old_marker[i];
            } else {
                video_memory[temp_pos + i] = (GREEN_ON_BLACK << 8) | ' ';
            }
        }
    }

    switch (c) {
        case '\r':
        case '\n': // Newline (line feed)
            if ((cursor_position / SCREEN_WIDTH) < SCREEN_HEIGHT - 1) {
                cursor_position += SCREEN_WIDTH - (cursor_position % SCREEN_WIDTH);
            }
            else { 
                scroll_or_reset();
            }
        break;
            
        case '\t': // Tab (move to next tab stop, assuming 8 spaces per tab)
            {
                int spaces = 8 - ((cursor_position % SCREEN_WIDTH) % 8);
                for (int i = 0; i < spaces; i++) {
                    if (cursor_position < SCREEN_WIDTH * SCREEN_HEIGHT) {
                        video_memory[cursor_position] = (GREEN_ON_BLACK << 8) | ' ';
                        cursor_position++;
                    } else {
                        scroll_or_reset();
                        break;
                    }
                }
            }
            break;
            
        case '\b': // Backspace
            if (cursor_position > 0) {
                cursor_position--;
                video_memory[cursor_position] = (GREEN_ON_BLACK << 8) | ' ';
            }
            break;
            
        default: // Regular character
            video_memory[cursor_position] = (GREEN_ON_BLACK << 8) | c;
            cursor_position++;
            break;
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

