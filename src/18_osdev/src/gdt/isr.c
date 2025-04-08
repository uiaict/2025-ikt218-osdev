#include "../../include/libc/common.h"
#include "isr.h"
#include "../../include/libc/monitor.h"

isr_t interrupt_handlers[256];

// this binds one irq to one handler function, like IRQ1 and keyboard_handler
void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

// function to handle interrupts, gets called from the interrupt.asm in the common stub
void isr_handler(registers_t regs)
{
   monitor_write("recieved interrupt: ");
   monitor_write_dec(regs.int_no);
   monitor_put('\n');
}

// function to handle interrupt requests, gets called from the interrupt.asm in the common stub
void irq_handler(registers_t regs)
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

   if (interrupt_handlers[regs.int_no] != 0)
   {
      isr_t handler = interrupt_handlers[regs.int_no];
      handler(regs);
   }
}


