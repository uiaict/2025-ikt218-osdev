#pragma once

#include "libc/stdint.h"

// Registers a custom handler function for a specific IRQ number
void irq_install_handler(int irq, void (*handler)());

// Unregisters a previously installed IRQ handler
void irq_uninstall_handler(int irq);

// Called by the interrupt handler to dispatch to the correct IRQ handler
void irq_handler(uint32_t irq_num);

// Initializes the PIC and remaps IRQs to avoid conflicts with CPU exceptions
void irq_init();
