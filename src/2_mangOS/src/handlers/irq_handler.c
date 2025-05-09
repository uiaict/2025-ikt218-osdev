#include "idt.h"
#include "libc/stdio.h"

void irq0_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
}

void irq2_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ2 triggered!\n");
}

void irq3_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ3 triggered!\n");
}

void irq4_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ4 triggered!\n");
}

void irq5_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ5 triggered!\n");
}

void irq6_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ6 triggered!\n");
}

void irq7_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ7 triggered!\n");
}

void irq8_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ8 triggered!\n");
}

void irq9_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ9 triggered!\n");
}

void irq10_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ10 triggered!\n");
}

void irq11_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ11 triggered!\n");
}

void irq12_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ12 triggered!\n");
}

void irq13_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ13 triggered!\n");
}

void irq14_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ14 triggered!\n");
}

void irq15_handler(registers_t *regs, void *ctx)
{
    // Handle the interrupt here
    printf("IRQ15 triggered!\n");
}

void init_irq_handlers()
{
    register_irq_handler(IRQ2, irq2_handler, NULL);
    register_irq_handler(IRQ3, irq3_handler, NULL);
    register_irq_handler(IRQ4, irq4_handler, NULL);
    register_irq_handler(IRQ5, irq5_handler, NULL);
    register_irq_handler(IRQ6, irq6_handler, NULL);
    register_irq_handler(IRQ7, irq7_handler, NULL);
    register_irq_handler(IRQ8, irq8_handler, NULL);
    register_irq_handler(IRQ9, irq9_handler, NULL);
    register_irq_handler(IRQ10, irq10_handler, NULL);
    register_irq_handler(IRQ11, irq11_handler, NULL);
    register_irq_handler(IRQ12, irq12_handler, NULL);
    register_irq_handler(IRQ13, irq13_handler, NULL);
    register_irq_handler(IRQ14, irq14_handler, NULL);
    register_irq_handler(IRQ15, irq15_handler, NULL);
}