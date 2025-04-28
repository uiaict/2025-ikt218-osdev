#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "terminal.h"
#include "port_io.h"
#include "keyboard.h"

static uint32_t timer_ticks = 0;

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
    if (r->int_no >= 40) {
        // Send reset signal to slave PIC
        outb(0xA0, 0x20);
    }
    // Send reset signal to master PIC
    outb(0x20, 0x20);

    if (r->int_no == 32) {  // IRQ0 (Timer interrupt)
        timer_ticks++;

        if (timer_ticks % 100 == 0) {
            terminal_printf("Timer Tick: %d\n", timer_ticks);
        }
    }
    else if (r->int_no == 33) {  // IRQ1 (Keyboard)
        keyboard_handler(r);
    } 
    else {
        terminal_printf("Received IRQ: %d\n", r->int_no - 32);
    }
}

// Remap the PIC
void irq_remap(void) {
    outb(0x20, 0x11); // Start initialization sequence (master)
    outb(0xA0, 0x11); // Start initialization sequence (slave)

    outb(0x21, 0x20); // Master offset to 0x20 (32)
    outb(0xA1, 0x28); // Slave offset to 0x28 (40)

    outb(0x21, 0x04); // Tell master there is a slave at IRQ2
    outb(0xA1, 0x02); // Tell slave its cascade identity

    outb(0x21, 0x01); // Master: 8086/88 (MCS-80/85) mode
    outb(0xA1, 0x01); // Slave: 8086/88 mode

    outb(0x21, 0x0);  // Unmask all master IRQs
    outb(0xA1, 0x0);  // Unmask all slave IRQs
}

// Install IRQ handlers to IDT
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
