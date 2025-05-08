#include "kernel/interrupts.h"
#include "common/io.h"
#include "libc/stddef.h"

// ──────────────────────────────────────────────────────────────
// IRQ HANDLER TABLE INITIALIZATION
// This sets all IRQ handlers to NULL at boot, making sure we start
// with a clean slate before any handler registration happens.
// ──────────────────────────────────────────────────────────────

void init_irq() {
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i].handler = NULL;
        irq_handlers[i].data = NULL;
        irq_handlers[i].num = i;
    }
}

// ──────────────────────────────────────────────────────────────
// REGISTER IRQ HANDLER
// Allows you to associate a C function with a hardware IRQ number.
// Later, when that IRQ fires, the registered function will be called.
// ──────────────────────────────────────────────────────────────

void register_irq_handler(int irq, isr_t handler, void* ctx) {
    if (irq >= 0 && irq < IRQ_COUNT) {
        irq_handlers[irq].handler = handler;
        irq_handlers[irq].data = ctx;
        irq_handlers[irq].num = irq;
    }
}

void unregister_irq_handler(int irq) {
  if (irq >= 0 && irq < IRQ_COUNT) {
      irq_handlers[irq].handler = NULL;
      irq_handlers[irq].data = NULL;
  }
}

// ──────────────────────────────────────────────────────────────
// IRQ HANDLER DISPATCHER
// Called from IRQ stubs in assembly after the CPU receives an IRQ.
// Handles sending an End-Of-Interrupt (EOI) to the PIC controllers
// and invokes any user-registered handler if available.
// ──────────────────────────────────────────────────────────────

void irq_handler(registers_t regs) {
    // Acknowledge the interrupt to the PIC(s)
    if (regs.int_no >= IRQ_BASE + 8) {
        outb(0xA0, 0x20);  // Acknowledge slave PIC
    }
    outb(0x20, 0x20);      // Always acknowledge master PIC

    // Convert interrupt number to index within the IRQ range
    int irq_index = regs.int_no - IRQ_BASE;
    if (irq_index >= 0 && irq_index < IRQ_COUNT) {
        struct int_handler_t intrpt = irq_handlers[irq_index];
        if (intrpt.handler != 0) {
            intrpt.handler(&regs, intrpt.data);
        }
    }
}
