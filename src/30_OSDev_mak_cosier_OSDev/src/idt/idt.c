#include "../include/libc/idt.h"
#include "../include/libc/isr.h"
#include "../include/libc/string.h"
#include "../include/libc/common.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);

void init_idt()
{
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint32_t) &idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);



    //0x20 commands & 0x21 data
    //0xA0 commands & 0xA1 data

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
    
    outb(0x20, 0X11);
    outb(0xA0, 0X11);

    outb(0x21, 0X20);
    outb(0xA1, 0X28);

    outb(0x21, 0X04);
    outb(0xA1, 0X02);

    outb(0x21, 0X01);
    outb(0xA1, 0X01);

    outb(0x21, 0X0);
    outb(0xA1, 0X0);

    // Setup IRQs (mapped to IDT entries 32–47)
    idt_set_gate(32, (u32int)irq0,  0x08, 0x8E);
    idt_set_gate(33, (u32int)irq1,  0x08, 0x8E);
    idt_set_gate(34, (u32int)irq2,  0x08, 0x8E);
    idt_set_gate(35, (u32int)irq3,  0x08, 0x8E);
    idt_set_gate(36, (u32int)irq4,  0x08, 0x8E);
    idt_set_gate(37, (u32int)irq5,  0x08, 0x8E);
    idt_set_gate(38, (u32int)irq6,  0x08, 0x8E);
    idt_set_gate(39, (u32int)irq7,  0x08, 0x8E);
    idt_set_gate(40, (u32int)irq8,  0x08, 0x8E);
    idt_set_gate(41, (u32int)irq9,  0x08, 0x8E);
    idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);

 




    idt_flush((uint32_t)&idt_ptr);
}


void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{

    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    
    idt_entries[num].always0 =0;
    idt_entries[num].flags = flags; //| 0x60;
}

//unsigned char* exception_messages[] = 
//{
//    "Division By Zero",
//    "Debug",
//    "Non Maskable Interrupt",
//    "Breakpoint",
//    "Into Detected Overflow",
//    "Out of Bounds",
//    "Invalid Opcode",
//    "No Coprocessor",
//    "Double fault",
//    "Coprocessor Segment Overrun",
//    "Bad TSS",
//    "Segment not present",
//    "Stack fault",
//    "General protection fault",
//    "Page fault",
//    "Unknown Interrupt",
//    "Coprocessor Fault",
//    "Alignment Fault", 
//    "Machine Check",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved",
//    "Reserved"
//};
    
    