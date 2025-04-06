
#include "libc/idt.h"


extern void isr_irq0();
extern void isr_irq1();
extern void isr_irq2();
extern void isr_irq3();
extern void isr_irq4();
extern void isr_irq5();
extern void isr_irq6();
extern void isr_irq7();
extern void isr_irq8();
extern void isr_irq9();
extern void isr_irq10();
extern void isr_irq11();
extern void isr_irq12();
extern void isr_irq13();
extern void isr_irq14();
extern void isr_irq15();


// IDT-tabellen og IDT-peker
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idt_descriptor;

// Funksjon for å sette en IDT-oppføring
void set_idt_entry(int index, uint32_t isr, uint16_t selector, uint8_t type_attr) {
    idt[index].offset_low = isr & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_high = (isr >> 16) & 0xFFFF;
}

extern void isr_syscall();
extern void isr_div_zero();
extern void default_isr();


void init_idt() {
    idt_descriptor.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    // 1. Nullstill IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_entry(i, 0, 0x08, 0x8E);
    }

    // 2. Sett opp faktiske ISR-er (IRQ0–IRQ15)
    set_idt_entry(0x00, (uint32_t)isr_div_zero, 0x08, 0x8E);
    set_idt_entry(0x20, (uint32_t)isr_irq0,  0x08, 0x8E);
    set_idt_entry(0x21, (uint32_t)isr_irq1,  0x08, 0x8E);
    set_idt_entry(0x22, (uint32_t)isr_irq2,  0x08, 0x8E);
    set_idt_entry(0x23, (uint32_t)isr_irq3,  0x08, 0x8E);
    set_idt_entry(0x24, (uint32_t)isr_irq4,  0x08, 0x8E);
    set_idt_entry(0x25, (uint32_t)isr_irq5,  0x08, 0x8E);
    set_idt_entry(0x26, (uint32_t)isr_irq6,  0x08, 0x8E);
    set_idt_entry(0x27, (uint32_t)isr_irq7,  0x08, 0x8E);
    set_idt_entry(0x28, (uint32_t)isr_irq8,  0x08, 0x8E);
    set_idt_entry(0x29, (uint32_t)isr_irq9,  0x08, 0x8E);
    set_idt_entry(0x2A, (uint32_t)isr_irq10, 0x08, 0x8E);
    set_idt_entry(0x2B, (uint32_t)isr_irq11, 0x08, 0x8E);
    set_idt_entry(0x2C, (uint32_t)isr_irq12, 0x08, 0x8E);
    set_idt_entry(0x2D, (uint32_t)isr_irq13, 0x08, 0x8E);
    set_idt_entry(0x2E, (uint32_t)isr_irq14, 0x08, 0x8E);
    set_idt_entry(0x2F, (uint32_t)isr_irq15, 0x08, 0x8E);
    set_idt_entry(0x80, (uint32_t)isr_syscall, 0x08, 0xEE);


    // 3. Sett default_isr for resterende oppføringer som fortsatt er 0
    for (int i = 0; i < IDT_SIZE; i++) {
        if (idt[i].offset_low == 0 && idt[i].offset_high == 0) {
            set_idt_entry(i, (uint32_t)default_isr, 0x08, 0x8E);
        }
    }

    // 4. Last IDT
    __asm__ volatile ("lidt (%0)" : : "r"(&idt_descriptor));
}


void remap_pic() {
    outb(0x20, 0x11); // Init master PIC
    outb(0xA0, 0x11); // Init slave PIC
    outb(0x21, 0x20); // Remap master til 0x20-0x27
    outb(0xA1, 0x28); // Remap slave til 0x28-0x2F
    outb(0x21, 0x04); // Cascade
    outb(0xA1, 0x02); // Cascade
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01); // 8086 mode
    outb(0x21, 0x0);  // Enable all IRQs on master
    outb(0xA1, 0x0);  // Enable all IRQs on slave
}






