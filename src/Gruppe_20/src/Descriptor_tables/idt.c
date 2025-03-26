#include "gdt_idt_table.h"
#include "libc/stdint.h"
#include "isr.h"


idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);
static void init_idt();
static void idt_set_gate(uint8_t , uint32_t , uint16_t , uint8_t );


extern void isr0();
extern void isr1();
extern void isr2();


void init_idt()
{
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) -1;
    idt_ptr.base = (uint32_t) &idt_entries;

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