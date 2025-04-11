#ifndef IDT_H
#define IDT_H

#include <libc/stdint.h>
#include "libc/isr.h"  // Includes definition for registers_t and related ISR types
#include <libc/stddef.h>            // Ensures standard types like size_t are available

// --- Define missing macros if not defined elsewhere ---

// Typical value for the kernel code segment in GDT (usually 0x08 in x86 OS development)
#define i686_GDT_CODE_SEGMENT 0x08

// For a 32-bit interrupt gate:
//  - 0x80: present bit is set (P = 1)
//  - 0x0E: denotes a 32-bit interrupt gate (TYPE = 0x0E)
// Combining these, we get 0x8E.
#define IDT_FLAG_GATE_32BIT_INT 0x8E

// For ring 0 (kernel mode): no extra bits are needed
#define IDT_FLAG_RING0 0x00

// --- Macro alias with proper casting ---
// This macro casts the ISR function pointer to uintptr_t before calling idt_set_gate.
#define i686_IDT_SetGate(num, base, sel, flags) \
    idt_set_gate((num), (uintptr_t)(base), (sel), (flags))

// --- IDT Structures ---

// An IDT entry: one entry in the Interrupt Descriptor Table.
struct idt_entry_struct {
    uint16_t base_low;   // Lower 16 bits of address of the interrupt handler.
    uint16_t sel;        // Kernel segment selector.
    uint8_t  always0;    // This must always be 0.
    uint8_t  flags;      // Type and attributes.
    uint16_t base_high;  // Upper 16 bits of the handler's address.
} __attribute__((packed));

// A structure describing a pointer to the IDT array; used with the lidt instruction.
struct idt_ptr_struct {
    uint16_t limit;
    uintptr_t base;      // Base address of the first element in the idt_entry_struct array.
} __attribute__((packed));

// --- Function Prototypes ---

// Initializes the IDT.
void init_idt();

// Sets an IDT entry.
//  - num   : the index in the IDT.
//  - base  : the address of the interrupt handler (now using uintptr_t).
//  - sel   : the kernel segment selector.
//  - flags : attributes for the entry (e.g., combination of IDT_FLAG_RING0 and IDT_FLAG_GATE_32BIT_INT).
void idt_set_gate(uint8_t num, uintptr_t base, uint16_t sel, uint8_t flags);

// --- External ISR Function Declarations ---
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

extern void isr128();
extern void isr177();

// --- External IRQ Function Declarations ---
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

#endif // IDT_H
