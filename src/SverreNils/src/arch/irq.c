#include "arch/irq.h"
#include "arch/idt.h"
#include "libc/io.h"
#include "printf.h"
#include "devices/keyboard.h"

#define MAX_IRQS 16
static void (*irq_handlers[MAX_IRQS])(void) = { 0 };

extern void* irq_stub_table[16];

// Remap PIC to avoid conflicts with CPU exceptions
static void pic_remap() {
    // Start init sequence
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Set new offsets
    outb(0x21, 0x20); // Master = 0x20 (32)
    outb(0xA1, 0x28); // Slave = 0x28 (40)

    // Tell master/slave relationship
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Set 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Clear all masks first
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    // Unmask IRQ1 only (keyboard)
    outb(0x21, 0xFD); // 11111101 = IRQ1 on, rest off
}

void irq_install() {
    pic_remap();

    for (int i = 0; i < 16; i++) {
        idt_set_gate(0x20 + i, (uint32_t)irq_stub_table[i], 0x08, 0x8E);
    }
}

void irq_handler(struct registers* regs) {
    int irq = regs->int_no - 0x20;
    if (irq >= 0 && irq < MAX_IRQS && irq_handlers[irq]) {
        irq_handlers[irq](); // Kaller registrert handler
    }

    if (regs->int_no >= 0x28)
        outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq_register_handler(int irq, void (*handler)(void)) {
    if (irq < MAX_IRQS) {
        irq_handlers[irq] = handler;
    }
}
