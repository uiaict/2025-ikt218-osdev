// isr.c
#include <libc/stdio.h>
#include <libc/stdint.h>
#include <print.h>
#include <libc/isr.h>
#include <libc/common.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}

// Match the declaration in isr.h
void isr_handler(registers_t regs) {
    printf("Received interrupt: %d, Error code: %d\n", regs.int_no, regs.err_code);
    
    // Halt the system if a critical exception occurs
    if (regs.int_no <= 31) {
        printf("SYSTEM HALTED: Exception %d\n", regs.int_no);
        for(;;); // Halt the CPU
    }
}

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
