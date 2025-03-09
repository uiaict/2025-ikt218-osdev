#include "drivers/terminal.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"

int col = 0;
int row = 0;

// https://wiki.osdev.org/Printing_To_Screen
uint16_t *video = 0xB8000;

void terminal_write(int color, const char *str) {
    
}

// https://www.geeksforgeeks.org/implement-itoa/
void char *itoa(int num, char *str, int base) {
    int i = 0;
    bool is_neg = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        is_neg = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_neg) {
        str[i++] = '-';
    }

    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char tmp = str[start];
        str[start] = str[end];
        str[end] = tmp;
        end--;
        start++;
    }

    return str;
}