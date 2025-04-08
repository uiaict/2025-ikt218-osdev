#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdarg.h>
#include "libc/stdint.h"

size_t strlen(const char* str);
void reverse(char* str, int length);
char* itoa(int num, char* str, int base);
void ftoa(float num, char *str, int afterpoint);
void memset(void* dest, char val, uint32_t count);
void outPortB(uint16_t port, uint8_t value);
char inPortB(uint16_t port);

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t rip,cs, rflags, userrsp, ss; // eip,cs, rflags, userrsp, ss if shit goes wrong with this maybe try these instead. I dunno
};