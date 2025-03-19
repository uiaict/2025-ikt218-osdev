#ifndef IDT_H
#define IDT_H
#include "libc/stdint.h"

// Define the number of IDT entries
#define IDT_ENTRIES 256

// Define the IDT entry structure
struct idt_entry_t {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void idt_flush(uint32_t);
void idt_load();

static struct idt_entry_t idt[IDT_ENTRIES];
static struct idt_ptr_t idt_ptr;

#endif