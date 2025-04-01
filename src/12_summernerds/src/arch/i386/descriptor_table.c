#include <stdint.h>

#define GDT_ENTRIES 5 //total number of entries in the gdt
#define IDT_ENTRIES 256 //same for the IDT

struct idt_entry {
    uint16_t base_low; //lower 16 bits of the handler function address
    uint16_t selector; //selector for segment
    uint8_t zero; //a byte set to constant zero
    uint8_t flags; //defines types/privileges of entry
    uint16_t base_high; //higher 16 bits of the handler function address
} __attribute__((packed));
// The line over makes sure that no zero-padding is added to this structure.

//Under we define pointers for gdt and idt:
struct idt_ptr {
    uint16_t limit; //IDT size
    uint32_t base; //Memory adress of IDT
} __attribute__((packed));


struct idt_entry idt[IDT_ENTRIES]; 
struct idt_ptr idt_descriptor; 

void set_idt_entry(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void init_idt() { 
    idt_descriptor.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_descriptor.base = (uint32_t)&idt;

    for (int i = 0; i < 32; i++) {
        set_idt_entry(i, (uint32_t)interrupt_handler, 0x08, 0x8E);
    }

    for (int i = 32; i < 48; i++) {
        set_idt_entry(i, (uint32_t)interrupt_handler, 0x08, 0x8E);
    }

    idt_load(&idt_descriptor);
}

void idt_load(struct idt_ptr *idt_ptr); 

void interrupt_handler() {
    
}

void int_handlers() {
}
