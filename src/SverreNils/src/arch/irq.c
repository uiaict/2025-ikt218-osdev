#include "arch/irq.h"
#include "arch/idt.h"
#include "libc/io.h"
#include "printf.h"

// Remap PIC to avoid conflicts with CPU exceptions
static void pic_remap() {
    outb(0x20, 0x11); // init master
    outb(0xA0, 0x11); // init slave
    outb(0x21, 0x20); // master offset = 0x20
    outb(0xA1, 0x28); // slave offset = 0x28
    outb(0x21, 0x04); // master tells there is slave at IRQ2
    outb(0xA1, 0x02); // slave identity
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01); // 8086 mode
    outb(0x21, 0x0);  // clear masks
    outb(0xA1, 0x0);  // clear masks
}

extern void* irq_stub_table[16];

void irq_install() {
    pic_remap();

    for (int i = 0; i < 16; i++) {
        idt_set_gate(0x20 + i, (uint32_t)irq_stub_table[i], 0x08, 0x8E);
    }
}

void irq_handler(struct registers* regs) {
    printf("IRQ %d triggered\n", regs->int_no - 0x20);

    // Send EOI (end of interrupt) to PIC
    if (regs->int_no >= 0x28)
        outb(0xA0, 0x20); // slave
    outb(0x20, 0x20);     // master
}
