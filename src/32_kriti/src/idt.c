// idt.c
#include "idt.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <kprint.h>  // Assuming you have this for printing

#define IDT_MAX_DESCRIPTORS 256

// IDT entry structure
typedef struct {
    uint16_t isr_low;      // Lower 16 bits of ISR address
    uint16_t kernel_cs;    // Code segment selector
    uint8_t reserved;      // Reserved, should be 0
    uint8_t attributes;    // Type and attributes
    uint16_t isr_high;     // Upper 16 bits of ISR address
} __attribute__((packed)) idt_entry_t;

// IDTR structure
typedef struct {
    uint16_t limit;        // Size of IDT - 1
    uint32_t base;         // Base address of IDT
} __attribute__((packed)) idtr_t;

// Define the IDT array and IDTR
static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;
static bool vectors[IDT_MAX_DESCRIPTORS];

// C handler function signatures - define these in a separate file
void isr_handler(uint8_t interrupt);

// Simple placeholder handler functions
void default_handler() {
    kprint("default handler triggered!");
}

// Set an IDT descriptor
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low    = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs  = 0x08; // Kernel code segment selector from your GDT
    descriptor->reserved   = 0;
    descriptor->attributes = flags;
    descriptor->isr_high   = (uint32_t)isr >> 16;
}

// Initialize the IDT
void idt_init(void) {
    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);

    // Initialize all entries with a default handler
    for (uint16_t vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++) {
        idt_set_descriptor(vector, default_handler, 0x8E);
        vectors[vector] = true;
    }

    // Load the IDT
    __asm__ volatile ("lidt %0" : : "m"(idtr));
    
    // Don't enable interrupts yet - you might want to do this later
    // __asm__ volatile ("sti");
}

// Register a specific interrupt handler
void idt_register_handler(uint8_t vector, void* handler) {
    idt_set_descriptor(vector, handler, 0x8E);
}
// In idt.c
void idt_set_interrupt_gate(uint8_t vector, void* isr_handler) {
    // Assuming you have a standard segment selector and flags for interrupt gates
    idt_set_descriptor(vector, isr_handler, 0x8E);
}