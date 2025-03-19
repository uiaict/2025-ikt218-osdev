#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"

struct InterruptRegisters {
    uint32_t cr2, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

#endif