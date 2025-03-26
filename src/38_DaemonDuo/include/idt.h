#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"
#include "terminal.h"

// IDT entry structure
struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Interrupt handler structure
//struct regs {
  //  uint32_t gs, fs, es, ds;
  //  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  //  uint32_t int_no, err_code;
  //  uint32_t eip, cs, eflags, useresp, ss;
//};

struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;  // CORRECT ORDER for pusha
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};


// Function prototypes
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();
void idt_load();
void pic_remap();
void enable_irq(uint8_t irq);
void disable_irq(uint8_t irq);

// External assembly ISRs
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

// External assembly IRQs
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

// C handlers for ISRs and IRQs
void isr_handler(struct regs *r);
void irq_handler(struct regs *r);

// Port I/O functions
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif // IDT_H