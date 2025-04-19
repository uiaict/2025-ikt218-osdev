
/*
//#include "libc/stdio.h"
#include "print.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"
//#include "libc/system.h"

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT= 25;

static int cursor_x = 0;
static int cursor_y = 0;

uint16_t* video = (uint16_t*)0xB8000;

void scroll_up(){
    // Move each row of text one row upwards
    for (int y = 1; y < VGA_HEIGHT; ++y){
        for (int x = 0; x< VGA_WIDTH; ++x){
            const int index = y * VGA_WIDTH + x; // Current index
            const int prev_index = (y - 1) * VGA_WIDTH + x;
            video[prev_index] = video[index];
        }
    }
    // Clear the last row
    const int last_row = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (int x = 0; x < VGA_WIDTH; ++x){
        video[last_row + x] = ' '|0x0F00;
    }
    cursor_y--;
}

void printf(char* str, ...){
    va_list args;
    va_start(args, str);
    while (*str != '\0') {
        if (*str == '%') {
            ++str;
            if (*str == 'd') {
                int value = va_arg(args, int);
                char* str = int_to_string(value, 0);
                while(*str != '\0'){
                    putchar(*str);
                    str++;
                }
            }
        } else {
            putchar(*str);
        }
        ++str;
    }
    va_end(args);
}

void putchar(char c){
        if(c == '\n'){
            cursor_x = 0; // Move to the cursor back to the start of the line if the character is a newline
            if (++cursor_y >= VGA_HEIGHT){
                scroll_up(); // Scroll up if the cursor reaches the bottom
            }
        }
        else{
            video[VGA_WIDTH*cursor_y + cursor_x] = (video[VGA_WIDTH*cursor_y+cursor_x] & 0xFF00) | c;
            cursor_x++;
        }
        if (cursor_x >= 80){
            cursor_x = 0; // Move back to the start of the line if the cursor reaches the end of the line
            if (++cursor_y >= VGA_HEIGHT){
                scroll_up(); // Scroll up if the cursor reaches the bottom
            }
            else{
                cursor_y++; //  Otherwise: move cursor to the next line
            }
        }
}

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

char* int_to_string(int num, char* str) {
    int i = 0;
    int n = num;
    int is_negative = 0;

    // If the number is 0, handle it as a special case
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers
    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    // Convert individual digits to characters
    while (n != 0) {
        int rem = n % 10;
        str[i++] = rem + '0';
        n = n / 10;
    }

    // Add sign if the number is negative
    if (is_negative) {
        str[i++] = '-';
    }

    // Add null terminator
    str[i] = '\0';

    // Reverse the string
    reverse(str, i);

    return str;
}

void panic(const char *msg) {
    write_to_terminal("PANIC: ", 1);
    write_to_terminal(msg, 1);
    while (1);  // Stopp alt
}

*/