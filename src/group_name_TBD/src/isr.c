#include "libc/stdint.h"
#include "isr.h"
#include "libc/stdio.h"
#include "io.h"

void isr_handler(struct registers reg){
    printf("ISR\n\r");
}

void irq_handler(struct registers reg){
    // IRQ0-7 / ISR32-39 master PIC
    // IRQ8-15 / ISR40-47 slave PIC
    printf("IRQ\n\r");
    

   if (reg.int_no > 39){
       // IRQ went through slave PIC
       // Needs End Of Interrupt signal
       outb(S_PIC_COMMAND, PIC_EOI);
   }
   // IRQ will always pass through master PIC,
   // so must always get End Of Interrupt signal
   outb(M_PIC_COMMAND, PIC_EOI);

//    if (interrupt_handlers[regs.int_no] != 0)
//    {
//        isr_t handler = interrupt_handlers[regs.int_no];
//        handler(regs);
//    }
}
