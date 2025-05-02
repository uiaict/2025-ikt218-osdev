#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "libc/stdint.h"

typedef struct registers {
  uint32_t ds; // Data segment selector
  uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
  uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
  uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
} registers_t;

void default_interrupt_handler();
void spurious_interrupt_handler();
void keyboard_handler();

void init_interrupts();

#endif