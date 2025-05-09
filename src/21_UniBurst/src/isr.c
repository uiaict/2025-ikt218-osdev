/* Adapted from the James Molloy's GDT/IDT implementation totorial at https://archive.is/L3pyA */

#include "libc/stdio.h"
#include "isr.h"
#include "io.h"

isr_t interruptHandlers[256];


void registerInterruptHandler(uint8_t n, isr_t handler) {
  interruptHandlers[n] = handler;
}


void isrHandler(registers_t regs) {
  if (interruptHandlers[regs.intNum] != 0) {
		isr_t handler = interruptHandlers[regs.intNum];
		handler(regs);
   	}

	else {
		printf("Received interupt: %d - %s\n", regs.intNum, exceptionMessages[regs.intNum]);
	}
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

