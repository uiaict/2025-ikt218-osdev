#ifndef IRQ_H
#define IRQ_H

#include "isr.h"  // contains the definition for registers_t

// Define a function pointer type for interrupt service routines (ISRs)
typedef void (*isr_t)(registers_t *regs);  // Use registers_t *regs consistently

// Global interrupt handler array (one per interrupt vector)
extern isr_t interrupt_handlers[256];

// Registers an ISR callback for a given interrupt number.
void register_interrupt_handler(unsigned char n, isr_t handler);

// This function is called by the common IRQ handler.
void irq_handler(registers_t *regs);

// Install an interrupt handler for a specific IRQ.
void irq_install_handler(int irq, void (*handler)(registers_t *regs))
{
    interrupt_handlers[irq] = handler;
}

#endif
