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
#define ISR32 32
#define ISR33 33    
#define ISR34 34
#define ISR35 35
#define ISR36 36
#define ISR37 37
#define ISR38 38
#define ISR39 39
#define ISR40 40
#define ISR41 41
#define ISR42 42
#define ISR43 43
#define ISR44 44
#define ISR45 45
#define ISR46 46
#define ISR47 47

#define IRQ_COUNT 16

#define IQR0 0, 32
#define IQR1 1, 33
#define IQR2 2, 34 
#define IQR3 3, 35
#define IQR4 4, 36
#define IQR5 5, 37
#define IQR6 6, 38
#define IQR7 7, 39
#define IQR8 8, 40
#define IQR9 9, 41
#define IQR10 10, 42
#define IQR11 11, 43
#define IQR12 12, 44
#define IQR13 13, 45
#define IQR14 14, 46
#define IQR15 15, 47


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