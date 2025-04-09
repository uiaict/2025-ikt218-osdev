#pragma once

#include <libc/stdint.h>
#include <libc/isr.h> // <-- Legg til denne!

typedef void (*isr_t)(registers_t *, void *); // OK

struct int_handler_t
{
    isr_t handler;
    void *data;
    int num;
};

#define IRQ_COUNT 256
extern struct int_handler_t irq_handlers[IRQ_COUNT];

void register_irq_handler(int irq, isr_t handler, void *ctx);
void irq_handler(uint32_t irq_number); // <-- ENDRET HER
void init_irq();
