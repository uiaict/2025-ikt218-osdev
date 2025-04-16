#include "gdt_idt_table.h"
#include "libc/stdint.h"
#include "libc/string.h"

#include "isr.h"
#include "idt.h"
#include "common.h"



extern void idt_flush(uint32_t idt_ptr);

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;


void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base = (uint32_t) &idt_entries;

    //memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

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

    idt_set_gate(0, (uint32_t) isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t) isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t) isr2, 0x08, 0x8E);
    idt_flush((uint32_t)&idt_ptr);
}
 
 void idt_set_gate(uint8_t num , uint32_t base, uint16_t sel, uint8_t flags)
 {
   idt_entries[num].base_low = base & 0xFFFF;
   idt_entries[num].base_high = (base >> 16) & 0xFFFF;

   idt_entries[num].select     = sel;
   idt_entries[num].always0 = 0;
   idt_entries[num].flags = flags; 
 }