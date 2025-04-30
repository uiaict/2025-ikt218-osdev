#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "libc/stdint.h"
#include "libc/stdio.h"
#include "libc/isr.h"


// ISR Definitions
#define ISR1 1
#define ISR2 2
#define ISR3 3
#define ISR4 4
#define ISR5 5
#define ISR6 6
#define ISR7 7
#define ISR8 8
#define ISR9 9
#define ISR10 10
#define ISR11 11
#define ISR12 12
#define ISR13 13
#define ISR14 14
#define ISR15 15
#define ISR16 16
#define ISR17 17
#define ISR18 18
#define ISR19 19
#define ISR20 20
#define ISR21 21
#define ISR22 22
#define ISR23 23
#define ISR24 24
#define ISR25 25
#define ISR26 26
#define ISR27 27
#define ISR28 28
#define ISR29 29
#define ISR30 30
#define ISR31 31

#define IRQ_COUNT 16

// Updated handler structure to match isr_t typedef
struct int_handler_t {
    int num;
    isr_t handler;
    void* context;  // Changed from 'data' to 'context' for clarity
};

extern struct int_handler_t int_handlers[IDT_ENTRIES];
extern struct int_handler_t irq_handlers[IRQ_COUNT];

void init_irq();
void init_interrupts();

// Helper function to register handlers
static inline void register_handler(struct int_handler_t* handlers, int num, 
                                  isr_t handler, void* context) {
    handlers[num].num = num;
    handlers[num].handler = handler;
    handlers[num].context = context;
}

#endif