#include "libc/stdint.h"
#include "idt.h"
#include "libc/io.h"


//Declate ISR stubs for interrupts 0, 1 and 2.
//These are functions from another file
//Void parameter means no parameters
extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);

extern void irq0();
extern void irq1();
extern void irq2();
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
extern void irq15();


//Declare an external function to flush the IDT from idt.asm
extern void idt_flush(uint32_t);

//Decalare an array of 256 IDT entries
__attribute__((aligned(0x10))) 
idt_entry_t idt_entries[256];


// Initialize the IDT
void init_idt(){

    //Set the IDTR strucure
    idtr.limit = sizeof(idt_entry_t) * 256 -1; //How big the IDT is in memory
    idtr.base  = (uint32_t)&idt_entries; //Address of the IDT in memory

    //Clear the entire table. set all entries to 0
    for (int i = 0; i < 256; i++) {
        idt_entries[i].isr_low = 0;
        idt_entries[i].isr_high = 0;
        idt_entries[i].kernel_cs = 0;
        idt_entries[i].attributes = 0;
        idt_entries[i].reserved = 0;
    }

    //Declare th function as extern
    extern void* isr_stub_table[];

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

    // Set up the IDT entries for interrupts 0, 1, and 2 using a loop
    for (uint8_t vector = 0; vector <= 2; vector++) {
        idt_set_gate(vector, (uint32_t)isr_stub_table[vector], 0x08, 0x8E);
    }

    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    //Load the IDT into the CPU and flushing it
    idt_flush((uint32_t)&idtr);
}

//Create one idt gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags){
    idt_entries[num].isr_low = base & 0xFFFF;             //Sets lower 16 bits of the address
    idt_entries[num].isr_high = (base >> 16) & 0xFFFF;    //Sets upper 16 bits of the address
    idt_entries[num].kernel_cs = sel;                     //Sets the kernel code segment selector
    idt_entries[num].reserved = 0;                        //Reserved, always set to 0
    idt_entries[num].attributes = flags;                  //Sets the attributes, tells the CPU how to handle the interrupt
}

// This function is called from isr.asm when an exception or interrupt happenms
//Stops the CPU permanently
__attribute__((noreturn))
void exception_handler(void);

//Inline assembly code to diable interrupts and stop the CPU
void exception_handler() {
    __asm__ volatile ("cli; hlt");  // "cli" disables interrupt, "hlt" stops the CPU
}


/*
//Debug function to print the interrupt number to verify it works
void isr_common_handler(int interrupt_number) {
    switch (interrupt_number) {
        case 0:
            print("Interrupt 0\n");
            break;
        case 1:
            print("Interrupt 1\n");
            break;
        case 2:
            print("Interrupt 2\n");
            break;
        default:
            printf("Unknown interrupt");
    }
}
*/

