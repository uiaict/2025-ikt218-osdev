#include "common.h"
#include "isr.h"
#include "monitor.h"

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{
   if (regs.int_no < 32)
   {
       monitor_write("Exception: ");
       monitor_write(exception_messages[regs.int_no]);
       monitor_write(" (Interrupt: ");
       monitor_write_dec(regs.int_no);
       monitor_write(")\n");
       
       if (regs.int_no == 13) {
           monitor_write("Error code: ");
           monitor_write_dec(regs.err_code);
           monitor_write("\n");
       }
       
       if (regs.int_no != 3)
       {
           monitor_write("System halted due to CPU exception\n");
           panic(exception_messages[regs.int_no]);
       }
   }
   else if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
   else
   {
       monitor_write("Received interrupt: ");
       monitor_write_dec(regs.int_no);
       monitor_put('\n');
   }
}

void irq_handler(registers_t regs)
{
   if (regs.int_no >= 40)
   {
       outb(0xA0, 0x20);
   }
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

void register_interrupt_handler(u8int n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}
