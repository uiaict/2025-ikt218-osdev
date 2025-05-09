#ifndef IRQ_H
#define IRQ_H

// IRQ Handlers
void irq0();
void irq1();
void irq2();

// Function to handle IRQs
void irq_handler(uint32_t irq_number);

#endif
