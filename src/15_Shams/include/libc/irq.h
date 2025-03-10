#ifndef IRQ_H
#define IRQ_H


#include "libc/stdint.h"   // Include standard integer types like uint16_t, uint32_t


// Funksjon for å initialisere IRQs
void init_irq();

// Handler for IRQs (blir kalt av assembler)
void irq_handler(uint32_t irq_number);

void keyboard_handler();


#endif /* IRQ_H */
