#include "libc/stdint.h" // for uint8_t, uint32_t
#include "libc/idt.h"
#include "libc/irq.h"

__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
    uint32_t isr_address = (uint32_t)isr;

    descriptor->isr_low    = isr_address & 0xFFFF;
    descriptor->kernel_cs  = 0x08; // Segment selector in GDT
    descriptor->reserved   = 0;
    descriptor->attributes = flags;
    descriptor->isr_high   = (isr_address >> 16) & 0xFFFF;
}

void idt_init(void) {
    pic_remap();

    idtr.base = (uint32_t)&idt[0];
    idtr.limit = sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t i = 0; i < 32; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
    }

    for (uint8_t i = 32; i <= 47; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
    }
    

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}
