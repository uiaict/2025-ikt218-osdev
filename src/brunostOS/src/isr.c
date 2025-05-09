#include "libc/stdint.h"
#include "isr.h"
#include "libc/stdio.h"
#include "io.h"
#include "libc/stddef.h"

// Array of function pointers.
// Contains specialized handlers
void (*isr_handlers[256])(struct registers); 

void register_interrupt_handler(uint8_t i, void (*specific_isr_handler)(struct registers)){

    isr_handlers[i] = specific_isr_handler;
} 





void isr_handler(struct registers *reg){
    // Used for exceptions
    if (isr_handlers[reg->int_no] != 0){
        // call handler for interrupt number "reg.int_no"
        // isr0 handler at index 0 etc.
        isr_handlers[reg->int_no](*reg);
   }
}

void irq_handler(struct registers *reg){
    // IRQ0-7 / ISR32-39 master PIC
    // IRQ8-15 / ISR40-47 slave PIC
    
   if (reg->int_no > IRQ7){
       // IRQ went through slave PIC, need EOI
       outb(S_PIC_COMMAND, PIC_EOI);
   }
   // IRQ will always pass through master PIC, master always need EOI
   outb(M_PIC_COMMAND, PIC_EOI);

   
   if (isr_handlers[reg->int_no] != 0){
        // call handler for interrupt number "reg.int_no"
        isr_handlers[reg->int_no](*reg);
   }
}
