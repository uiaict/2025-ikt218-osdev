#pragma once

#include <libc/stdint.h>
#include <libc/idt.h>
#include <libc/isr.h>

// Function pointer type for IRQ handlers
typedef void (*irq_handler_func_t)(registers_t regs);

// Initialize IRQ system
void init_irq(void);

// Register an IRQ handler
void register_irq_handler(uint8_t irq, irq_handler_func_t handler);

// Process IRQ and call the appropriate handler
void handle_irq(registers_t regs);

// Send EOI signal to acknowledge the interrupt
void irq_ack(uint8_t irq);
