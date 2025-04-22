/* Adapted from the James Molloy's GDT/IDT implementation totorial at https://archive.is/L3pyA */

#include "libc/stdio.h"
#include "isr.h"
#include "io.h"

isr_t interruptHandlers[256];


void registerInterruptHandler(uint8_t n, isr_t handler) {
  interruptHandlers[n] = handler;
}


void isrHandler(registers_t regs) {
	terminal_write("Received interrupt: ");
  char int_str[3];
  int32_to_str(int_str, regs.intNum);
  terminal_write(int_str);
  terminal_write(" - ");
  terminal_write(exceptionMessages[regs.intNum]);
  terminal_write("\n");
}

// IRQ handler. Handles hardware interrupts
void irqHandler(registers_t regs) {
    if (regs.intNum >= 40) {

        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);

    if (interruptHandlers[regs.intNum] != 0) {
       isr_t handler = interruptHandlers[regs.intNum];
       handler(regs);
   }
}

