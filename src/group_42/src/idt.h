#ifndef IDT_H
#define IDT_H

#include "libc/stdint.h"

// IDT entry structure
typedef struct {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_ptr_t;

#define IDT_ENTRIES 256
extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idt_ptr;

// Function declarations
void set_idt_entry(int n, uint32_t handler);
void load_idt();

#endif