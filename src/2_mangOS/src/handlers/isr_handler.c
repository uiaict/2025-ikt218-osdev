#include "idt.h"
#include "libc/stdio.h"

void isr3_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("ISR3 triggered!\n");
}

void isr4_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("ISR4 triggered!\n");
}

void isr5_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("ISR5 triggered!\n");
}

void init_isr_handlers()
{
    register_interrupt_handler(ISR3, isr3_handler, NULL);
    register_interrupt_handler(ISR4, isr4_handler, NULL);
    register_interrupt_handler(ISR5, isr5_handler, NULL);
}