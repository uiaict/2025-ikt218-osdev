#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdarg.h>

#define VGA_ADDRESS 0xB8000
#define VGA_ROWS 25
#define VGA_COLUMS 80

void printf(const char* str, ...);
void printChar(int color, char c);
void kernel_write(int color, const char* str);