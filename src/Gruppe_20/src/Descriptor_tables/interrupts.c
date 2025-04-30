#include "interrupts.h"

// Allocate arrays for handlers
struct int_handler_t int_handlers[IDT_ENTRIES];
struct int_handler_t irq_handlers[IRQ_COUNT];

void init_interrupts() {
    for (int i = 0; i < IDT_ENTRIES; i++) {
        int_handlers[i].handler = 0;
        int_handlers[i].context = 0;
    }
}

void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = 0;
        irq_handlers[i].context = 0;
    }
}
