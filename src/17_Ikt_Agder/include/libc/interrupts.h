#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <libc/stdint.h>

// Defines the interrupt gate descriptor structure (for IDT)
struct interrupt_gate {
    uint16_t offset_low;   // Low 16 bits of the interrupt handler address
    uint16_t selector;     // Segment selector
    uint8_t  zero;         // Reserved
    uint8_t  type_attr;    // Type and attributes of the interrupt gate
    uint16_t offset_high;  // High 16 bits of the interrupt handler address
} __attribute__((packed));


// Declare the PIT handler so it can be used in interrupts.c
void pit_handler();

// Function prototype for interrupt handlers
typedef void (*interrupt_handler_t)(void);

// Function to set the interrupt vector in the IDT
void set_interrupt_vector(uint8_t vector, interrupt_handler_t handler);

// Function to initialize the interrupt descriptor table (IDT)
void init_idt(void);

// Function to register interrupt handler for a specific IRQ (interrupt)
void register_interrupt_handler(int irq, void (*handler)());

// Function to handle interrupts (to be called by the interrupt dispatcher)
void interrupt_handler(void);

// Function to load the IDT
void load_idt(void);

// Initialize the interrupt system
void init_interrupts();

#endif // INTERRUPTS_H
