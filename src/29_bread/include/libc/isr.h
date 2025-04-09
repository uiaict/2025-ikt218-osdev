#pragma once


typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

// Declare ISR handlers from your isr.asm
extern void isr0();
extern void isr1();
extern void isr2();

// Optional: if you have a shared C handler
void isr_handler(uint32_t int_no);

void irq_handler(registers_t regs);

