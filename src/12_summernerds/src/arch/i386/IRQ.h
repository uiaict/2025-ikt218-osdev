#pragma once


#ifndef IRQ_H
#define IRQ_H


#define IRQ_COUNT 16

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47
#define IRQ_COUNT 16


typedef void (*isr_t)(registers_t);

// Initialize IRQ handlers
void init_irq();

// Register an IRQ handler

void register_interrupt_handler(uint8_t n, isr_t handler);

// The main IRQ handler
void irq_handler(registers_t regs);

/*


// Typedef for IRQ-handler
typedef void (*irq_handler_t)(void);

// Funksjoner som brukes i andre filer
void init_irq();
void register_irq_handler(int irq, irq_handler_t handler);
void irq_install_handler(int irq, void (*handler)(struct InterruptRegister *r));
void irq_handler(int irq);
//void irq_handler(struct InterruptRegister *regs)

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

*/

#endif
