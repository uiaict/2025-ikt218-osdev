#include "i386/interruptRegister.h"
#include "common.h"

// Initializing the IRQ handlers
void init_irq()
{
    for (int i = 0; i < IRQ_COUNT; i++)
    {
        irq_handlers[i].data = NULL;
        irq_handlers[i].handler = NULL;
        irq_handlers[i].num = i;
    }
}

// Registering the interrupts
void register_irq_handler(int irq, isr_t handler, void *ctx)
{
    irq_handlers[irq - 32].handler = handler;
    irq_handlers[irq - 32].data = ctx;
}

// Main IRQ handler, called from assembly defined irq0-15 funcs
void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20); // Send reset signal to slave.
    }
    outb(0x20, 0x20); // Send reset signal to master.

    printf("IRQ %d triggered\n", regs.int_no);

    // Call the IRQ handler
    int irq = regs.int_no - 32;
    if (irq < 0 || irq > IRQ_COUNT)
        return;

    struct int_handler_t intrpt = irq_handlers[irq];
    if (intrpt.handler)
    {
        intrpt.handler(&regs, intrpt.data);
    }
}
