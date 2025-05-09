#include "interrupts.h"
#include "common.h"

// Initialize IRQ handlers
void init_irq() {
  for (int i = 0; i < IRQ_COUNT; i++) {
    irq_handlers[i].data = NULL;
    irq_handlers[i].handler = NULL;
    irq_handlers[i].num = i;
  }
}

// Register an IRQ handler
void register_irq_handler(int irq, isr_t handler, void* ctx) {
  irq_handlers[irq].handler = handler;
  irq_handlers[irq].data = ctx;
}

// The main IRQ handler
// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
    // Send EOI (End of Interrupt) to PIC
    if (regs.int_no >= 40) {
        outb(0xA0, 0x20); // Slave PIC
    }
    outb(0x20, 0x20);     // Master PIC

    uint8_t irq = regs.int_no - 32;

    struct int_handler_t intrpt = irq_handlers[irq];
    if (intrpt.handler != 0) {
        intrpt.handler(&regs, intrpt.data);
    } //else {
        //monitor_writestring("Unhandled IRQ: ");
        //monitor_write_dec(irq);
        //monitor_put('\n');
    //}
}
