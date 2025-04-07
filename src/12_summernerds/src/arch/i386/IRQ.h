#pragma once


#ifndef IRQ_H
#define IRQ_H

#define IRQ_COUNT 16

// Typedef for IRQ-handler
typedef void (*irq_handler_t)(void);

// Funksjoner som brukes i andre filer
void init_irq();
void register_irq_handler(int irq, irq_handler_t handler);
void irq_handler(int irq);

// Ekstern tilgang til handler-arrayen (valgfritt)
extern irq_handler_t irq_handlers[IRQ_COUNT];

#endif
