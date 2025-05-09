#include "libc/irq.h"
#include "libc/common.h"
#include "libc/ports.h"  // Make sure this defines inb() and outb()
#include "libc/stdio.h"  // For debugging if needed

#include "libc/stdint.h"


// Initialize the global interrupt handler array to all NULLs.
isr_t interrupt_handlers[256] = { 0 };



void irq_handler(registers_t *regs) 
{
    // If a custom handler is registered for this IRQ, call it.
    if (interrupt_handlers[regs->int_no]) 
    {
        interrupt_handlers[regs->int_no](regs);
    }

    // If the interrupt number is from the slave PIC (IRQ8–15), send reset signal to the slave.
    if (regs->int_no >= 40) 
    {
        outb(0xA0, 0x20);
    }
    // Always send reset signal to the master PIC.
    outb(0x20, 0x20);

    
   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

void register_interrupt_handler(unsigned char n, isr_t handler) 
{
    interrupt_handlers[n] = handler;
}