#include "libc/stdint.h"

#define GDT_ENTRIES 5


struct gdt_entry{
    // 1 entry is 8 Bytes
    uint16_t limit_low;
    uint16_t base_low; 
    uint8_t base_middle;
    uint8_t access;         // 4bit pres, priv, type, && 4bit Type flags
    uint8_t granularity;    // 4bit Other flags && 4bit junk (1111)
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr{
    // Properties for the eventual gdt_entry array
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_gdt();
void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);
extern void gdt_flush(uint32_t*);

//========================================= GDT =========================================
//========================================= IDT =========================================

#define IDT_ENTRIES 256


struct idt_entry {
    uint16_t base_low;  // lower 16bit of adress to jump to if interrupt
    uint16_t selector;  // segment selector
    uint8_t zero;       // is just 0
    uint8_t flags;
    uint16_t base_high; // higher 16bit of adress to jump to if interrupt
} __attribute__((packed));
  
struct idt_ptr {
    // Properties for the eventual idt_entry array
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


void init_idt();
void idt_set_gate(int32_t, uint32_t, uint32_t, uint8_t);
extern void idt_flush(uint32_t*);

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

extern void irq0(); //isr32
extern void irq1(); //isr33
extern void irq2(); //etc
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
extern void irq15(); //isr47