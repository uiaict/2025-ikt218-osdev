#include "descriptorTables.h"
#include "interupts.h"
#include "common.h"
#include "libc/stddef.h"

struct idtEntry idt[IDT_ENTRIES];
struct idtPtr ip;

void initIdt() {
    ip.limit = sizeof(struct idtEntry) * IDT_ENTRIES - 1;
    ip.base = (uint32_t) &idt;
  
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idtSetGate(i, 0x00000000, 0x00);
        intHandlers[i].handler = NULL;
    }
  
    initInterrupts();
  
    // Load the IDT
    idtFlush((uint32_t)&ip);
    
  }

void idtSetGate(int32_t num, uint32_t base, uint8_t flags)
{
    idt[num].baseLow = (uint32_t)base & 0xFFFF;
    idt[num].baseHigh = (uint32_t)base >> 16;

    idt[num].selector = 0x08;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void idtFlush(uint32_t idtPtr) {
    asm volatile(
        "lidt (%0)\n"        // Load IDT pointer
        :
        : "r" (idtPtr)
        : "memory"
    );
}

void initInterrupts() {
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

    idtSetGate( 0, (uint32_t)isr0 , 0x8E);
    idtSetGate( 1, (uint32_t)isr1 , 0x8E);
    idtSetGate( 2, (uint32_t)isr2 , 0x8E);
    idtSetGate( 3, (uint32_t)isr3 , 0x8E);
    idtSetGate( 4, (uint32_t)isr4 , 0x8E);
    
    idtSetGate(32, (uint32_t)irq0, 0x8E);
    idtSetGate(33, (uint32_t)irq1, 0x8E);
    idtSetGate(34, (uint32_t)irq2, 0x8E);
    idtSetGate(35, (uint32_t)irq3, 0x8E);
    idtSetGate(36, (uint32_t)irq4, 0x8E);
    idtSetGate(37, (uint32_t)irq5, 0x8E);
    idtSetGate(38, (uint32_t)irq6, 0x8E);
    idtSetGate(39, (uint32_t)irq7, 0x8E);
    idtSetGate(40, (uint32_t)irq8, 0x8E);
    idtSetGate(41, (uint32_t)irq9, 0x8E);
    idtSetGate(42, (uint32_t)irq10, 0x8E);
    idtSetGate(43, (uint32_t)irq11, 0x8E);
    idtSetGate(44, (uint32_t)irq12, 0x8E);
    idtSetGate(45, (uint32_t)irq13, 0x8E);
    idtSetGate(46, (uint32_t)irq14, 0x8E);
    idtSetGate(47, (uint32_t)irq15, 0x8E);
}