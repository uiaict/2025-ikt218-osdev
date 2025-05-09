/* Adapted from James Molloy's GDT/IDT tutorial: https://archive.is/L3pyA */
#ifndef DESTABLES_H
#define DESTABLES_H

#include <libc/stdint.h>

// Initializes both the GDT and IDT
void initDesTables();

// Initializes the Global Descriptor Table (GDT)
void initGdt();

// Initializes the Interrupt Descriptor Table (IDT)
void initIdt();

// Represents a single GDT entry (used to define segment properties)
struct gdtEntryStruct {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__((packed));
typedef struct gdtEntryStruct gdtEntry_t;

// Pointer to the GDT (used by LGDT instruction)
struct gdtPtrStruct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct gdtPtrStruct gdtPtr_t;

// Represents a single IDT entry (used to define an interrupt handler)
struct idtEntryStruct {
    uint16_t baseLow;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t baseHigh;
} __attribute__((packed));
typedef struct idtEntryStruct idtEntry_t;

// Pointer to the IDT (used by LIDT instruction)
struct idtPtrStruct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idtPtrStruct idtPtr_t;

// CPU Exception Handlers (Interrupt Service Routines 0–31)
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();

// IRQ Handlers (Interrupt Requests from hardware devices, IRQ0–IRQ15)
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10 ();
extern void irq11 ();
extern void irq12 ();
extern void irq13 ();
extern void irq14 ();
extern void irq15 ();

#endif // DESTABLES_H
