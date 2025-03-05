#include "stdint.h"
#include "../teminal.h"  // Eller hvor disse funksjonene er definert


struct InterruptRegisters 
{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};

void isr_handler(struct InterruptRegisters* regs);
