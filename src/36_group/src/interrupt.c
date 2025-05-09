#include "interrupt.h"
#include "io.h"
#include "libc/stdio.h"

#define MAX_INTERRUPTS 256

static void (*interrupt_handlers[MAX_INTERRUPTS])(registers_t *);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

// External ISR/IRQ handlers (defined in interrupt.asm)
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void irq0_handler(void);
extern void irq1_handler(void);
extern void irq2_handler(void);
extern void irq3_handler(void);
extern void irq4_handler(void);
extern void irq5_handler(void);
extern void irq6_handler(void);
extern void irq7_handler(void);
extern void irq8_handler(void);
extern void irq9_handler(void);
extern void irq10_handler(void);
extern void irq11_handler(void);
extern void irq12_handler(void);
extern void irq13_handler(void);
extern void irq14_handler(void);
extern void irq15_handler(void);

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void init_idt(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    // Set ISRs for interrupts 0, 1, 2
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E); // Division by zero
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E); // Debug
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E); // Non-maskable interrupt

    // Set IRQ handlers (interrupts 32–47)
    idt_set_gate(32, (uint32_t)irq0_handler, 0x08, 0x8E); // PIT
    idt_set_gate(33, (uint32_t)irq1_handler, 0x08, 0x8E); // Keyboard
    idt_set_gate(34, (uint32_t)irq2_handler, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3_handler, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4_handler, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5_handler, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6_handler, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7_handler, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8_handler, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9_handler, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10_handler, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11_handler, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12_handler, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13_handler, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14_handler, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15_handler, 0x08, 0x8E);

    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idtp));
    puts("IDT initialized.\n");
}

void init_irq(void)
{
    // Remap PIC to interrupts 32–47
    outb(0x20, 0x11); // Initialize master PIC
    outb(0xA0, 0x11); // Initialize slave PIC
    outb(0x21, 0x20); // Master PIC vector offset (32)
    outb(0xA1, 0x28); // Slave PIC vector offset (40)
    outb(0x21, 0x04); // Tell master PIC about slave at IRQ2
    outb(0xA1, 0x02); // Tell slave PIC its cascade identity
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01);

    // Mask IRQ0 (PIT timer), enable IRQ1 (keyboard), mask others
    outb(0x21, 0xFC); // 0xFD = 11111101 (IRQ0 masked, IRQ1 enabled)
    outb(0xA1, 0xFF); // 0xFF = 11111111 (all slave IRQs masked)

    puts("IRQs initialized.\n");
}

void isr_handler(uint8_t interrupt)
{
    if (interrupt_handlers[interrupt])
    {
        registers_t r;
        interrupt_handlers[interrupt](&r);
    }
    else
    {
        printf("Unhandled ISR: Interrupt %d\n", (uint32_t)interrupt);
    }
}

void irq_handler(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(0xA0, 0x20); // Send EOI to slave
    }
    outb(0x20, 0x20); // Send EOI to master
    // printf("Received IRQ%d\n", (uint32_t)irq); // Debug
    if (interrupt_handlers[irq + 32])
    {
        registers_t r;
        interrupt_handlers[irq + 32](&r);
    }
    else
    {
        printf("Unhandled IRQ: IRQ%d\n", (uint32_t)irq);
    }
}

void register_interrupt_handler(uint8_t n, void (*handler)(registers_t *r))
{
    interrupt_handlers[n] = handler;
}

void isr0_handler(registers_t *r)
{
    (void)r;
    puts("Interrupt 0 (Divide by Zero) handled\n");
}

void isr1_handler(registers_t *r)
{
    (void)r;
    puts("Interrupt 1 (Debug) handled\n");
}

void isr2_handler(registers_t *r)
{
    (void)r;
    puts("Interrupt 2 (NMI) handled\n");
}