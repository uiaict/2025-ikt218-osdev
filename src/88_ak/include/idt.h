#ifndef IDT_H
#define IDT_H

#include "libc/system.h"
#include "utils.h"
#include "printf.h"

typedef struct {
    uint16_t base_low;  // lav 16 biter av rutineadresse
    uint16_t selector;  // GDT-selector
    uint8_t zero;       // alltid 0
    uint8_t flags;      // type, DPL, present-bit
    uint16_t base_high; // høy 16 biter av adresse
} __attribute__((packed)) idt_entry_struct;

typedef struct {
    uint16_t limit;     // størrelse av IDT (bytes - 1)
    uint32_t base;      // baseadresse til idt_entries
} __attribute__((packed)) idt_ptr_struct;

// Struktur med CPU-registre ved avbrudd
struct InterruptRegisters {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void initIdt();
void setIdtGate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void isr_handler(struct InterruptRegisters *regs);
void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r));
void irq_uninstall_handler(int irq);
void irq_handler(struct InterruptRegisters *regs);
void irq_routine(struct InterruptRegisters *regs);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

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

#endif