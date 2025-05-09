#include "interrupts.h"
#include "common.h"

// Fetch idt_flush function
extern void idt_flush(uint32_t);

// Start the IDT 
void start_idt() {
  idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
  idt_ptr.base = (uint32_t) &idt;

  for (int i = 0; i < IDT_ENTRIES; i++) {
    idt[i].low = 0x0000;
    idt[i].high = 0x0000;
    idt[i].selector = 0x08;
    idt[i].zero = 0x00;
    idt[i].flags = 0x8E;
    int_controllers[i].controller = NULL;
  }
  // Start the interrupts controller
  start_interrupts();
  idt_flush((uint32_t)&idt_ptr);
}
// Load the IDT
void idt_load() {
  asm volatile("lidt %0" : : "m" (idt_ptr));
}

// Interupt gate function to set up the IDT values
void interrupt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].low = base & 0xFFFF;
  idt[num].high = (base >> 16) & 0xFFFF;
  idt[num].selector = sel;
  idt[num].zero = 0;
  idt[num].flags = flags | 0x60;
}
// Start the interrupts
void start_interrupts() {

  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

// Map ISRs and IRQs to the IDT
  interrupt_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
  interrupt_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
  interrupt_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
  interrupt_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
  interrupt_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
  interrupt_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
  interrupt_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
  interrupt_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
  interrupt_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
  interrupt_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
  interrupt_gate(10, (uint32_t)isr10, 0x08, 0x8E);
  interrupt_gate(11, (uint32_t)isr11, 0x08, 0x8E);
  interrupt_gate(12, (uint32_t)isr12, 0x08, 0x8E);
  interrupt_gate(13, (uint32_t)isr13, 0x08, 0x8E);
  interrupt_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  interrupt_gate(15, (uint32_t)isr15, 0x08, 0x8E);
  interrupt_gate(16, (uint32_t)isr16, 0x08, 0x8E);
  interrupt_gate(17, (uint32_t)isr17, 0x08, 0x8E);
  interrupt_gate(18, (uint32_t)isr18, 0x08, 0x8E);
  interrupt_gate(19, (uint32_t)isr19, 0x08, 0x8E);
  interrupt_gate(20, (uint32_t)isr20, 0x08, 0x8E);
  interrupt_gate(21, (uint32_t)isr21, 0x08, 0x8E);
  interrupt_gate(22, (uint32_t)isr22, 0x08, 0x8E);
  interrupt_gate(23, (uint32_t)isr23, 0x08, 0x8E);
  interrupt_gate(24, (uint32_t)isr24, 0x08, 0x8E);
  interrupt_gate(25, (uint32_t)isr25, 0x08, 0x8E);
  interrupt_gate(26, (uint32_t)isr26, 0x08, 0x8E);
  interrupt_gate(27, (uint32_t)isr27, 0x08, 0x8E);
  interrupt_gate(28, (uint32_t)isr28, 0x08, 0x8E);
  interrupt_gate(29, (uint32_t)isr29, 0x08, 0x8E);
  interrupt_gate(30, (uint32_t)isr30, 0x08, 0x8E);
  interrupt_gate(31, (uint32_t)isr31, 0x08, 0x8E);
  interrupt_gate(32, (uint32_t)irq0, 0x08, 0x8E);
  interrupt_gate(33, (uint32_t)irq1, 0x08, 0x8E);
  interrupt_gate(34, (uint32_t)irq2, 0x08, 0x8E);
  interrupt_gate(35, (uint32_t)irq3, 0x08, 0x8E);
  interrupt_gate(36, (uint32_t)irq4, 0x08, 0x8E);
  interrupt_gate(37, (uint32_t)irq5, 0x08, 0x8E);
  interrupt_gate(38, (uint32_t)irq6, 0x08, 0x8E);
  interrupt_gate(39, (uint32_t)irq7, 0x08, 0x8E);
  interrupt_gate(40, (uint32_t)irq8, 0x08, 0x8E);
  interrupt_gate(41, (uint32_t)irq9, 0x08, 0x8E);
  interrupt_gate(42, (uint32_t)irq10, 0x08, 0x8E);
  interrupt_gate(43, (uint32_t)irq11, 0x08, 0x8E);
  interrupt_gate(44, (uint32_t)irq12, 0x08, 0x8E);
  interrupt_gate(45, (uint32_t)irq13, 0x08, 0x8E);
  interrupt_gate(46, (uint32_t)irq14, 0x08, 0x8E);
  interrupt_gate(47, (uint32_t)irq15, 0x08, 0x8E);
}