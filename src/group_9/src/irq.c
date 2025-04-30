#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "terminal.h"
#include "port_io.h"
#include "keyboard.h"

static uint32_t timer_ticks = 0;

// Array of IRQ handler function pointers
static void (*irq_routines[16])(struct regs *r) = { 0 };

// Extern IRQ function declarations (from irq.asm)
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Low-level IRQ handler called by assembly
void irq_handler(struct regs *r) {
    // Send End of Interrupt (EOI) to the PICs
    if (r->int_no >= 40) {
        outb(0xA0, 0x20); // Acknowledge slave PIC
    }
    outb(0x20, 0x20);     // Acknowledge master PIC

    uint8_t irq = r->int_no - 32;

    // If a custom handler is installed, call it
    if (irq_routines[irq]) {
        irq_routines[irq](r);
    } else {
        // No custom handler, handle basic interrupts
        if (irq == 0) { // Timer
            timer_ticks++;

            if (timer_ticks % 100 == 0) {
                terminal_printf("Timer Tick: %d\n", timer_ticks);
            }
        } else if (irq == 1) { // Keyboard
            keyboard_handler(r);
        } else {
            terminal_printf("Received IRQ: %d\n", irq);
        }
    }
}

// Remap the PIC to avoid conflicts with CPU exceptions
void irq_remap(void) {
    outb(0x20, 0x11); // Start initialization (master)
    outb(0xA0, 0x11); // Start initialization (slave)

    outb(0x21, 0x20); // Master offset 0x20
    outb(0xA1, 0x28); // Slave offset 0x28

    outb(0x21, 0x04); // Tell Master about Slave at IRQ2
    outb(0xA1, 0x02); // Tell Slave its cascade identity

    outb(0x21, 0x01); // Master: 8086/88 mode
    outb(0xA1, 0x01); // Slave: 8086/88 mode

    outb(0x21, 0x0);  // Unmask all interrupts on Master
    outb(0xA1, 0x0);  // Unmask all interrupts on Slave
}

// Install IRQ handlers into the IDT
void irq_install() {
    irq_remap();

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    __asm__ __volatile__("sti"); // Enable interrupts
}

// Install a custom IRQ handler
void irq_install_handler(int irq, void (*handler)(struct regs *r)) {
    irq_routines[irq] = handler;
}
