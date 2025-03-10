#include "libc/stdio.h"
#include "drivers/terminal.h"

int printf(const char* __restrict__ format, ...) {
    terminal_write(WHITE, format);
    return 0;
}