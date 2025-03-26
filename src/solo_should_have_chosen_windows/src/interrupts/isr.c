#include "terminal/print.h"
#include "interrupts/isr.h"
#include "libc/io.h"

void irq_handler(int irq) {
    static int counter = 0;

    if (irq < 32) {
        printf("Exception: Interupt (%d) - %x\n", irq, irq);
    }
    else if (irq >= 0x20 && irq <= 0x2F) {
        if (counter % 50 == 0)
            printf("IRQ %d (mapped to vector %d): Interrupt - %x\n", irq - 0x20, irq, irq);
        counter++;
    }
    else {
        printf("Other interrupt (%d) - %x\n", irq, irq);
    }
    

    // Acknowledge the PIC (IRQ0–IRQ15)
    if (irq >= 0x20 && irq <= 0x2F) {
        outb(0x20, 0x20); // Master PIC
        if (irq >= 0x28) {
            outb(0xA0, 0x20); // Also slave PIC
        }
    }
}