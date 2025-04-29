#pragma once

#include <stdint.h>
#include "arch/isr.h" // For struct registers

// Registrer en IRQ-handler (typisk IRQ0-15)
void irq_register_handler(int irq, void (*handler)(struct registers* regs));

// Fjern en tidligere registrert IRQ-handler
void irq_uninstall_handler(int irq);

// Hoved IRQ-dispatcher, kalles fra isr.asm (via irq_stub_table)
void irq_handler(struct registers* regs);

// Initialiserer IRQ-systemet (remapper PIC og setter IDT entries)
void irq_install(void);
