#pragma once
#include <stdint.h>

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;
    uint32_t err_code;
} registers_t;

void isr_handler(registers_t* regs);
