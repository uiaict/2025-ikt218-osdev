#include "interrupts/irq.h"
#include "interrupts/io.h"
#include "interrupts/pic.h"
#include "libc/stdio.h"

#define NUM_IRQS 16

static void (*irq_handlers[NUM_IRQS])() = { 0 };

void irq_install_handler(int irq, void (*handler)()) {
    if (irq < NUM_IRQS) {
        irq_handlers[irq] = handler;
    }
}

void irq_uninstall_handler(int irq) {
    if (irq < NUM_IRQS) {
        irq_handlers[irq] = 0;
    }
}

void irq_handler(uint32_t irq_num) {
    if (irq_num >= 32 && irq_num < 32 + NUM_IRQS) {
        int irq = irq_num - 32;
        if (irq_handlers[irq]) {
            irq_handlers[irq]();
            printf("IRQ received!\n");
        }
    }

    // Send EOI uansett
    if (irq_num >= 40) {
        outb(0xA0, 0x20); // Slave PIC
    }
    outb(0x20, 0x20);     // Master PIC
}

