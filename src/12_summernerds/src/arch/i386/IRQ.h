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



#endif
