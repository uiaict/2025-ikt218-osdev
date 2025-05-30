/* ---------------------------------------------------------------------
    * Some of this code is adapted from JamesM's kernel development 
    tutorials https://archive.is/8W6ew
    ---------------------------------------------------------------------
*/
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "descTables.h"
#include "isr.h"
#include "terminal.h"
#include "global.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

// ---------- ISR Handlers ----------
void isr_handler(registers_t regs)
{
  // printf("Received interrupt: %d\n", regs.int_no);
}

// ---------- IRQ Handlers ----------
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

