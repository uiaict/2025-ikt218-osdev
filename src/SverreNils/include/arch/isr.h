#pragma once
#include <stdint.h>

#define ISR_COUNT 256

struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void isr_install();
void isr_handler(struct registers* regs);
