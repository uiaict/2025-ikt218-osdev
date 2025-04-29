#include "arch/irq.h"
#include "arch/idt.h"
#include "libc/io.h"
#include "printf.h"

#define MAX_IRQS 16


static void (*irq_handlers[MAX_IRQS])(struct registers* regs) = { 0 };

extern void* irq_stub_table[16];


static void pic_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20); 
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);


    outb(0x21, 0x00);
    outb(0xA1, 0x00);
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
        irq_handlers[irq](regs); 
    }

   
    if (regs->int_no >= 0x28)
        outb(0xA0, 0x20);
    outb(0x20, 0x20);
}


void irq_register_handler(int irq, void (*handler)(struct registers* regs)) {
    if (irq < MAX_IRQS) {
        irq_handlers[irq] = handler;
    }
}
