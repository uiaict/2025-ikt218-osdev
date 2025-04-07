#include <libc/stdint.h>

//void clear_screen();
void putchar(char c);
void printf(char* str, ...);
void reverse(char str[], int length);

char* int_to_string(int num, char* str);