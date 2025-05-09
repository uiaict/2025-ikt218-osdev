/* Adapted from the James Molloy's GDT/IDT implementation totorial at https://archive.is/L3pyA */

#include "libc/stdio.h"
#include "interrupts/isr.h"
#include "interrupts/io.h"

isr_t interruptHandlers[256];

// Registers a custom handler for a specific interrupt number
void registerInterruptHandler(uint8_t n, isr_t handler) {
	interruptHandlers[n] = handler;
}

// Handles CPU exceptions (ISR 0–31)
void isrHandler(registers_t regs) {
	// If a handler is registered, call it
	if (interruptHandlers[regs.intNum] != 0) {
		isr_t handler = interruptHandlers[regs.intNum];
		handler(regs);
	}
	// Otherwise, print the interrupt number and message
	else {
		printf("Received interrupt: %d - %s\n", regs.intNum, exceptionMessages[regs.intNum]);
	}
}

// Handles hardware interrupts (IRQ 0–15)
void irqHandler(registers_t regs) {
	// Send EOI to slave PIC if needed
	if (regs.intNum >= 40) {
		outb(0xA0, 0x20);
	}
	// Always send EOI to master PIC
	outb(0x20, 0x20);

	// If a handler is registered, call it
	if (interruptHandlers[regs.intNum] != 0) {
		isr_t handler = interruptHandlers[regs.intNum];
		handler(regs);
	}
}
