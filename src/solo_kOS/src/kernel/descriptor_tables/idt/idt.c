#include "libc/stddef.h"
#include "kernel/interrupts.h"
#include "common/io.h"  // Not needed yet, but kept if you later re-add IRQs

// Defined in idt_flush.asm
extern void idt_flush(uint32_t);

//declare the static functions needt for interrupt initialization
static void init_interrupts(void);
static void remap_pic(void);
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Actual IDT and pointer (declared extern in idt.h)
struct idt_entry_t idt[IDT_ENTRIES];
struct idt_ptr_t idt_ptr;

/**
 * Initializes the Interrupt Descriptor Table (IDT) and loads it into the CPU.
 * Only the first three entries are set to ISRs (for testing software interrupts).
 */
void init_idt() {
    idt_ptr.limit = sizeof(struct idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base  = (uint32_t)&idt;

    // Clear all entries initially
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // initialize the interrupts
    init_interrupts();


    // Load the IDT
    idt_flush((uint32_t)&idt_ptr);
}

/**
 * Sets an entry in the IDT.
 * @param num The interrupt number (0-255)
 * @param base Address of the ISR
 * @param sel GDT segment selector (usually 0x08 for kernel code segment)
 * @param flags Gate flags (0x8E = present, ring 0, 32-bit interrupt gate)
 */
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

static void init_interrupts() {
    remap_pic(); // Remap IRQs

    // Register IRQs 0–15
    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
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
}


static void remap_pic() {
    uint8_t mask1 = inb(0x21); // Save masks
    uint8_t mask2 = inb(0xA1);

    outb(0x20, 0x11); // Start init (ICW1)
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // Master offset -> 0x20 (IRQ0–7)
    outb(0xA1, 0x28); // Slave offset -> 0x28 (IRQ8–15)
    outb(0x21, 0x04); // Tell Master: Slave at IRQ2
    outb(0xA1, 0x02); // Tell Slave: cascade identity
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01);
    outb(0x21, mask1); // Restore saved masks
    outb(0xA1, mask2);
}