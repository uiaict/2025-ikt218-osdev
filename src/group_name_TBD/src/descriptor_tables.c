#include "libc/stdint.h"
#include "descriptor_tables.h"
#include "isr.h"

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
    
    // All IDT entries must be set for the CPU to not crash and burn.
    // memset(&idt, 0, sizeof(struct idt_entry) * 256); 
    for (size_t i = 0; i < IDT_ENTRIES; i++){
        idt_set_gate(0, 0x00000000, 0x08, 0x8E); // Set all 256 entries as default
    }

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

