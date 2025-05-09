#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "terminal.h"
#include "keyboard.h"
#include "system.h"
#include "common.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

static isr_t irq_routines[16] = { 0 };

extern void* irq_stub_table[];
extern void set_idt_entry(int i, uint32_t base, uint16_t selector, uint8_t flags);

static void remap_pic() {
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);
    outb(PIC1_DATA, 0x20); // remap IRQ0–7 to 0x20–0x27
    outb(PIC2_DATA, 0x28); // remap IRQ8–15 to 0x28–0x2F
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, 0xF8); // allow IRQ0 (timer), IRQ1 (keyboard), and IRQ2 (cascade)
    outb(PIC2_DATA, 0xFF); // Mask all slave IRQs (fine for now)
}

void irq_install_handler(int irq, isr_t handler) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_install() {
    remap_pic();
    for (int i = 0; i < 16; ++i) {
        set_idt_entry(32 + i, (uint32_t)irq_stub_table[i], 0x08, 0x8E);
    }
}

void irq_handler(int irq, registers_t regs) {
    if (irq >= 40) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);

    int irq_number = irq - 32;
    if (irq_number < 0 || irq_number >= 16) return;

    if (irq_routines[irq_number]) {
        irq_routines[irq_number](regs);
    }
}
