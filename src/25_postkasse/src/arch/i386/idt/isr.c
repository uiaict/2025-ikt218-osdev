#include "libc/stdint.h"
#include "libc/io.h"
#include "isr.h"
#include "libc/monitor.h"

// Array to store interrupt handlers
isr_t interrupt_handlers[256];

//Print to screen when an interrupt occurs
void isr_handler(registers_t regs) {
    monitor_write("Interrupt");
}

//Function is called from isr.asm when an IRQ occurs
void irq_handler(registers_t regs){
    // If this interrupt involved the slave
   if (regs.int_no >= 40){
       outb(0xA0, 0x20); //Sends reset signal to slave PIC
   }

   outb(0x20, 0x20);     //Sends reset signal to master PIC

    
   if (interrupt_handlers[regs.int_no] != 0){
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

// Function to register an interrupt handler
void register_interrupt_handler(uint8_t n, isr_t handler){
  interrupt_handlers[n] = handler;
}