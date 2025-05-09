#ifndef IRQ_H
#define IRQ_H

#include "libc/stdint.h"
#include "interrupts.h"

// Function to initialize all IRQ handlers
void init_irq(void);

void irq_controller(registers_t* regs);


// Function to register a specific IRQ handler
void register_irq_handler(uint8_t irq, isr_t handler, void* context);

#endif
