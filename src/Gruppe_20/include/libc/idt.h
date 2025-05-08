#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// IDT entry structure
typedef struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;


// Function declarations
void init_idt();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Assembly function to load IDT
extern void idt_flush(uint32_t idt_ptr_address);

#ifdef __cplusplus
}
#endif

#endif // IDT_H
