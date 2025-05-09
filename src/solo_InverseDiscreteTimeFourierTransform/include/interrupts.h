#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "libc/stdint.h"
#include "descriptor_tables.h"

#define ISR1        1
#define ISR2        2
#define ISR3        3

#define IRQ0       32
#define IRQ1       33
#define IRQ2       34
#define IRQ3       35
#define IRQ4       36
#define IRQ5       37
#define IRQ6       38
#define IRQ7       39
#define IRQ8       40
#define IRQ9       41
#define IRQ10      42
#define IRQ11      43
#define IRQ12      44
#define IRQ13      45
#define IRQ14      46
#define IRQ15      47
#define IDT_ENTRIES 256

// One 8‑byte IDT entry
struct idt_entry_t {
    uint16_t offset_low; 
    uint16_t selector;  
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

// Pointer structure passed to lidt
struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern struct idt_entry_t idt_entries[IDT_ENTRIES];
extern struct idt_ptr_t   idt_ptr;


extern void idt_load(uint32_t);

// Assembly ISR stubs
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);

// IRQ stubs
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);



// Init those three ISRs and load IDT
void init_interrupts(void);

void init_irq(void);

// common C handler called by each stub,
void isr_common(int int_no);

void register_irq_handler(int irq, void (*handler)(int, void*), void* ctx);

#endif