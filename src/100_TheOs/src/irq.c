#include "interrupts.h"
#include "common.h"

// Start of the IRQ controller
void start_irq() {
  for (int i = 0; i < IRQ_COUNT; i++) {
    irq_controllers[i].data = NULL;
    irq_controllers[i].controller = NULL;
    irq_controllers[i].num = i;
  }
}

// Register Controller (IRQ)
void register_irq_controller(int irq, isr_t controller, void* ctx) {
  irq_controllers[irq].controller = controller;
  irq_controllers[irq].data = ctx;
}

// IRQ controller function
void irq_controller(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);

    struct int_controller_t intrpt = irq_controllers[regs.int_no - 32];
    if (intrpt.controller != 0)
    {
        intrpt.controller(&regs, intrpt.data);
    }

}