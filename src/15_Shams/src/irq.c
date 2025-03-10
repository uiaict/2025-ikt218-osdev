#include <libc/irq.h>
#include <libc/idt.h>
#include <libc/terminal.h>

extern void irq0();
extern void irq1();
extern void irq2();



void irq_handler(uint32_t irq_number) {
    terminal_write("Received IRQ: ");
    terminal_putc('0' + irq_number);
    terminal_write("\n");
}

void init_irq() {
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);
}

void keyboard_handler() {
    terminal_write("Keyboard interrupt triggered!\n");
}


