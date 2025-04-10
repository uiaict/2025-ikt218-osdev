#pragma once

#include <stdint.h>

// Type for IRQ handlers
typedef void (*irq_handler_t)(void);

// Main IRQ handler (called from ASM stub)
void irq_handler(int irq_no, int err_code);

// Function to register a custom handler for an IRQ
void irq_register_handler(int irq, irq_handler_t handler);

// Initialization function to set up IRQs
void irq_install();

// Declare IRQ stubs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
