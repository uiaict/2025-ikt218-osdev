#include "libc/stdint.h"
#include "../include/teminal.h"



#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define WHITE_ON_BLACK 0x0F

uint16_t* terminal_buffer = (uint16_t*)VGA_ADDRESS;
int term_row = 0;
int term_col = 0;

void terminal_putc(char c) 
{
    if (c == '\n') 
    {
        term_row++;
        term_col = 0;
        return;
    }
    terminal_buffer[term_row * VGA_WIDTH + term_col] = (WHITE_ON_BLACK << 8) | c;
    term_col++;
}

void printf(const char* format, ...) 
{
    for (int i = 0; format[i] != '\0'; i++) 
    {
        terminal_putc(format[i]);
    }
}






void print_number(int num) {
    char buffer[12];  // Enough for a 32-bit int (-2147483648 to 2147483647) + null terminator
    int i = 0, is_negative = 0;

    if (num == 0) {
        terminal_putc('0');
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;  // Convert to positive
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';  // Convert last digit to character
        num /= 10;
    }

    if (is_negative) {
        buffer[i++] = '-';  // Add negative sign
    }

    buffer[i] = '\0';  // Null-terminate the string

    // Reverse the string since digits were stored in reverse order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    // Print the string one character at a time.
    for (int j = 0; buffer[j] != '\0'; j++) {
        terminal_putc(buffer[j]);
    }
}
