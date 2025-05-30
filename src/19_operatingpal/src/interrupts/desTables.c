/* Adapted from the James Molloy's GDT/IDT implementation totorial at https://archive.is/L3pyA */

#include "interrupts/desTables.h"
#include "libc/stdint.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "interrupts/isr.h"

extern void gdtFlush(uint32_t gdtPtr);
extern void idtFlush(uint32_t idtPtr);

static void gdtSetGate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);
static void idtSetGate(uint8_t,uint32_t,uint16_t,uint8_t);

gdtEntry_t gdtEntries[5];
idtEntry_t idtEntries[256];
gdtPtr_t gdtPtr;
idtPtr_t idtPtr;

// Initializes both GDT and IDT
void initDesTables() {
    initGdt();
    initIdt();
    asm volatile("sti");
}

// Initializes the GDT with 5 entries
void initGdt() {
    gdtPtr.limit = (sizeof(gdtEntry_t) * 5) - 1;
    gdtPtr.base  = (uint32_t)&gdtEntries;
    
    gdtSetGate(0, 0, 0, 0, 0);                
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 
    gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 
    gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 

    gdtFlush((uint32_t)&gdtPtr);
}

// Sets a single GDT entry
static void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdtEntries[num].baseLow = (base & 0xFFFF);
    gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
    gdtEntries[num].baseHigh = (base >> 24) & 0xFF;
    gdtEntries[num].limitLow = (limit & 0xFFFF);
    gdtEntries[num].granularity = (limit >> 16) & 0x0F;
    gdtEntries[num].granularity |= gran & 0xF0;
    gdtEntries[num].access = access;
}

// Initializes the IDT with ISRs and IRQs
void initIdt() {
    idtPtr.limit = (sizeof(idtEntry_t) * 256) - 1;
    idtPtr.base  = (uint32_t)&idtEntries;

    memset(&idtEntries, 0, sizeof(idtEntry_t) * 256);

    // Remap PIC
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

    // Set ISRs (0–31)
    idtSetGate(0, (uint32_t)isr0, 0x08, 0x8E);
    idtSetGate(1, (uint32_t)isr1, 0x08, 0x8E);
    idtSetGate(2, (uint32_t)isr2, 0x08, 0x8E);
    idtSetGate(3, (uint32_t)isr3, 0x08, 0x8E);
    idtSetGate(4, (uint32_t)isr4, 0x08, 0x8E);
    idtSetGate(5, (uint32_t)isr5, 0x08, 0x8E);
    idtSetGate(6, (uint32_t)isr6, 0x08, 0x8E);
    idtSetGate(7, (uint32_t)isr7, 0x08, 0x8E);
    idtSetGate(8, (uint32_t)isr8, 0x08, 0x8E);
    idtSetGate(9, (uint32_t)isr9, 0x08, 0x8E);
    idtSetGate(10, (uint32_t)isr10, 0x08, 0x8E);
    idtSetGate(11, (uint32_t)isr11, 0x08, 0x8E);
    idtSetGate(12, (uint32_t)isr12, 0x08, 0x8E);
    idtSetGate(13, (uint32_t)isr13, 0x08, 0x8E);
    idtSetGate(14, (uint32_t)isr14, 0x08, 0x8E);
    idtSetGate(15, (uint32_t)isr15, 0x08, 0x8E);
    idtSetGate(16, (uint32_t)isr16, 0x08, 0x8E);
    idtSetGate(17, (uint32_t)isr17, 0x08, 0x8E);
    idtSetGate(18, (uint32_t)isr18, 0x08, 0x8E);
    idtSetGate(19, (uint32_t)isr19, 0x08, 0x8E);
    idtSetGate(20, (uint32_t)isr20, 0x08, 0x8E);
    idtSetGate(21, (uint32_t)isr21, 0x08, 0x8E);
    idtSetGate(22, (uint32_t)isr22, 0x08, 0x8E);
    idtSetGate(23, (uint32_t)isr23, 0x08, 0x8E);
    idtSetGate(24, (uint32_t)isr24, 0x08, 0x8E);
    idtSetGate(25, (uint32_t)isr25, 0x08, 0x8E);
    idtSetGate(26, (uint32_t)isr26, 0x08, 0x8E);
    idtSetGate(27, (uint32_t)isr27, 0x08, 0x8E);
    idtSetGate(28, (uint32_t)isr28, 0x08, 0x8E);
    idtSetGate(29, (uint32_t)isr29, 0x08, 0x8E);
    idtSetGate(30, (uint32_t)isr30, 0x08, 0x8E);
    idtSetGate(31, (uint32_t)isr31, 0x08, 0x8E);

    // Set IRQs (32–47)
    idtSetGate(32, (uint32_t)irq0, 0x08, 0x8E);
    idtSetGate(33, (uint32_t)irq1, 0x08, 0x8E);
    idtSetGate(34, (uint32_t)irq2, 0x08, 0x8E);
    idtSetGate(35, (uint32_t)irq3, 0x08, 0x8E);
    idtSetGate(36, (uint32_t)irq4, 0x08, 0x8E);
    idtSetGate(37, (uint32_t)irq5, 0x08, 0x8E);
    idtSetGate(38, (uint32_t)irq6, 0x08, 0x8E);
    idtSetGate(39, (uint32_t)irq7, 0x08, 0x8E);
    idtSetGate(40, (uint32_t)irq8, 0x08, 0x8E);
    idtSetGate(41, (uint32_t)irq9, 0x08, 0x8E);
    idtSetGate(42, (uint32_t)irq10, 0x08, 0x8E);
    idtSetGate(43, (uint32_t)irq11, 0x08, 0x8E);
    idtSetGate(44, (uint32_t)irq12, 0x08, 0x8E);
    idtSetGate(45, (uint32_t)irq13, 0x08, 0x8E);
    idtSetGate(46, (uint32_t)irq14, 0x08, 0x8E);
    idtSetGate(47, (uint32_t)irq15, 0x08, 0x8E);

    idtFlush((uint32_t)&idtPtr);
}

// Sets a single IDT entry
static void idtSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idtEntries[num].baseLow = base & 0xFFFF;
    idtEntries[num].baseHigh = (base >> 16) & 0xFFFF;
    idtEntries[num].sel = sel;
    idtEntries[num].always0 = 0;
    idtEntries[num].flags = flags;
}
