#include "libc/stdint.h"
#include "descriptor_tables.h"
#include "isr.h"
#include "io.h"
#include "memory/memutils.h"

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdt_ptr;


void init_gdt(){
    
    // Store base and limit properties for gdt[]
    gdt_ptr.limit = sizeof(struct gdt_entry) * GDT_ENTRIES - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // num, base, limit, access, granularity
    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment     9A, CF = 1001 1010, 1100 1111
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment     92, CF = 1001 0010, 1100 1111
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment, [[maybe_unused]]
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment, [[maybe_unused]] 		

    // Flush GDT pointer
    gdt_flush((uint32_t)&gdt_ptr); 
}


void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    
    gdt[num].base_low = (base & 0xFFFF);            // Exctract first 16bit from base with bitwise AND. 0xFFFF = 2^16
    gdt[num].base_middle = (base >> 16) & 0xFF;     // Shift away the first 16bit, extract "first" 8bit from base
    gdt[num].base_high = (base >> 24) & 0xFF;       // Shift away the first 24bit, extract "first" 8bit from base

    gdt[num].limit_low = (limit & 0xFFFF);          // Exctract first 16bit from limit with bitwise AND. 0xFFFF = 2^16
    gdt[num].granularity = (limit >> 16) & 0x0F;    // ??Sets granularity to 0x00??

    gdt[num].granularity |= gran & 0xF0;            // ??adds 0 to end of gran? bitwise OR with granularity pointless as it is 0x00??
    gdt[num].access = access;
}


//========================================= GDT =========================================
//========================================= IDT =========================================


struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptr;



void init_idt() {
    // Store base and limit properties for idt[]
    idt_ptr.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t) &idt;
    
    
// Remap the irq table.
// ICW: Initialization Command Words
// Arbitrary numbers meaning specific things to the PIC

    // Gives command to master and slave PIC
    outb(M_PIC_COMMAND, 0x10 | 0x01); // ICW_INIT + tells ICW4 will be used
    outb(S_PIC_COMMAND, 0x10 | 0x01); //   (ICW1)
    
    // Sends data to master and slave PIC
    outb(M_PIC_DATA, 0x20);     // Offset to shift IRQ0
    outb(S_PIC_DATA, 0x28);     //   from ISR0 to ISR32
    
    outb(M_PIC_DATA, 0x04);     // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(S_PIC_DATA, 0x02);     // Tell Slave PIC its cascade identity (0000 0010)
    
    outb(M_PIC_DATA, 0x01);     // Makes PIC use 8086 mode
    outb(S_PIC_DATA, 0x01);     //   instead of 8080 mode (ICW4)
    
    outb(M_PIC_DATA, 0xFC);     // Only unmansk IRQ1 for keyboard interrupts
    outb(S_PIC_DATA, 0xFF);     //   and IRQ0 for PIT (0xFC=11111100)
    
    asm volatile("sti");        // Enables hardware interrupts.
// IRQs 0..15 correspond to ISRs 32..47 (31 being the last CPU-used ISR)
    
    
    
    
    // All IDT entries must be set for the CPU to not crash and burn.
    memset(&idt, 0, sizeof(struct idt_entry) * 256); // Set all 256 entries as 0

    // ISR used by CPU
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
  
    // ISR used by IRQ
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E); //isr32
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E); //isr33
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E); //isr34
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E); //etc
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E); //isr39
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E); //isr40
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E); //isr47
    
    
    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
  }
  
  
void idt_set_gate(int32_t num, uint32_t base, uint32_t selector, uint8_t flags){
    
    idt[num].base_low = (base & 0xFFFF);        // Exctract first 16bit from base with bitwise AND. 0xFFFF = 2^16
    idt[num].base_high = (base >> 16) & 0xFFFF; // Shift away the first 16bit, extract "first" 16bit from base
    idt[num].selector = 0x08;                   // base of code segment in GDT
    idt[num].zero = 0x00;                       // Always 0
    idt[num].flags = flags;                     // 8E = 10001110, se doc for flag meaning
}

