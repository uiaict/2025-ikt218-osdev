#include "terminal/print.h"
#include "interrupts/isr.h"
#include "libc/io.h"

void irq_handler(int irq) {
    static int counter = 0;
    counter++;
    if (counter % 50 == 0)
        printf("IRQ %d fired\n", irq);

    // Acknowledge the PIC (IRQ0–IRQ15)
    if (irq >= 0x00 && irq <= 0x0F) {
        outb(0x20, 0x20); // Master PIC
        if (irq >= 8) {
            outb(0xA0, 0x20); // Also slave PIC
        }
    }
}