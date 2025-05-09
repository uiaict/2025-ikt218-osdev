#include <libc/irq.h>
#include <libc/idt.h>
#include <libc/terminal.h>
#include <libc/isr.h>
#include <libc/io.h>

extern void irq0();
extern void irq1();
extern void irq2();

void irq_handler(uint32_t irq_number)
{
    registers_t regs;
    regs.int_no = irq_number + 32;

    isr_handler(regs);
}

// Remapper PIC slik at IRQ0–IRQ15 blir interrupt 32–47
void remap_pic()
{
    // Initialiseringskommandoer til master og slave PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Fortell PIC hvor i IDT tabellen den skal begynne
    outb(0x21, 0x20); // Master PIC: avbryt på int 32
    outb(0xA1, 0x28); // Slave PIC: avbryt på int 40

    // Konfigurer chaining
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Angi 8086/88 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Skru på alle IRQ-er igjen
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void init_irq()
{

    remap_pic();
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
}
