#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"
#include "gdt.h"
#include "libc/descriptor_tables.h"


#ifdef __cplusplus
extern "C" {
#endif

// IDT entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

 //IDT pointer structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

// IDT Gate Types
#define IDT_TASK_GATE    0x5
#define IDT_INT_GATE     0xE  // Interrupt gate (clears IF)
#define IDT_TRAP_GATE    0xF  // Trap gate (preserves IF)

// IDT Flags
#define IDT_FLAG_PRESENT  (1 << 7)
#define IDT_FLAG_RING0    (0 << 5)
#define IDT_FLAG_RING3    (3 << 5)

// Initialize IDT
void init_idt();

// Set individual IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Load IDT (assembly function)
extern void idt_flush(uint32_t ptr);
#ifdef __cplusplus
}
#endif

#endif // IDT_H