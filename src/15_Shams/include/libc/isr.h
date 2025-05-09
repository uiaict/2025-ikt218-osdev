#ifndef ISR_H
#define ISR_H

#include <libc/stdint.h>

typedef struct
{
    uint32_t ds;                                     // Data segment
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushet av pusha
    uint32_t int_no, err_code;                       // Interrupt nummer + feilkode
    uint32_t eip, cs, eflags, useresp, ss;           // CPU state
} registers_t;

void isr_install();
void isr_handler(registers_t regs);
void register_interrupt_handler(uint8_t n, void (*handler)(registers_t));

#endif
