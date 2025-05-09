#include "kernel/idt.h"

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void set_idt_entry(int n, uint32_t handler) {
  idt[n].offset_low = handler & 0xFFFF;
  idt[n].selector = 0x08;
  idt[n].zero = 0;
  idt[n].type_attr = 0x8E;
  idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void load_idt() {
  idt_ptr.limit = sizeof(idt) - 1;
  idt_ptr.base = (uint32_t)&idt;

  // Load the IDT using the lidt instruction:
  __asm__ volatile("lidt (%0)" : : "r"(&idt_ptr));
}