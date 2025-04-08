#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdarg.h>

#define VGA_ADDRESS 0xB8000
#define VGA_ROWS 25
#define VGA_COLUMNS 80
#define COLOR8_BLACK 0
#define COLOR8_LIGHT_GREY 7

void reset();
void newLine();
void scrollUp();
void printf(const char* str, ...);