#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdarg.h>

size_t strlen(const char* str);
void reverse(char* str, int length);
char* itoa(int num, char* str, int base);
void ftoa(float num, char *str, int afterpoint);
void memset(void* dest, char val, uint32_t count);