#ifndef ISR_H
#define ISR_H

#include "libc/stdint.h"

typedef struct registers {
    uint32_t ds;                            // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;             // Interrupt number and error code (some ISRs don't push err_code)
    uint32_t eip, cs, eflags, useresp, ss; // Automatically pushed by the CPU
} registers_t;

#endif
