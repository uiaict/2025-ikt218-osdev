
#include "libc/stdint.h" // For uint8_t og uint16_t
// io.h
//#pragma once
//#include "libc/stdint.h" // For uint8_t og uint16_t

void memset (void *dest, char val, uint32_t count);
void outPortB(uint16_t Port, uint8_t Value);
char inPortB(uint16_t port);

/*
struct InterruptRegister{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;

    };

*/
