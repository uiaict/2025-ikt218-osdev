#include "interupts.h"
#include "common.h"
#include "libc/stddef.h"

// Initialize IRQ handlers
void initIrq() {
  for (int i = 0; i < IRQ_COUNT; i++) {
    irqHandlers[i].data = NULL;
    irqHandlers[i].handler = NULL;
    irqHandlers[i].num = i;
  }
}

// Register an IRQ handler
void registerIrqHandler(int irq, isr_t handler, void* ctx) {
  irqHandlers[irq].handler = handler;
  irqHandlers[irq].data = ctx;
}

// The main IRQ handler
// This gets called from our ASM interrupt handler stub.
void irqHandler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    struct interuptHandlerT intrpt = irqHandlers[regs.int_no];
    if (intrpt.handler != 0)
    {
        intrpt.handler(&regs, intrpt.data);
    }

}