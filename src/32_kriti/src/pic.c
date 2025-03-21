#include "pic.h"
#include <libc/stdint.h>

// Helper functions for port I/O are now defined as static inline in the header file

void pic_init() {
    // Save masks
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    // ICW1: start initialization sequence in cascade mode
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    
    // ICW2: Define PIC vectors
    outb(PIC1_DATA, PIC1_OFFSET); // IRQ 0-7 -> INT 0x20-0x27
    outb(PIC2_DATA, PIC2_OFFSET); // IRQ 8-15 -> INT 0x28-0x2F
    
    // ICW3: Tell Master PIC that there is a slave PIC at IRQ2
    outb(PIC1_DATA, 0x04);
    
    // ICW3: Tell Slave PIC its cascade identity
    outb(PIC2_DATA, 0x02);
    
    // ICW4: Request 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    
    // Restore masks - except enable keyboard (IRQ1)
    mask1 &= ~(1 << 1);  // Unmask keyboard IRQ
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        // If this is from slave PIC, send EOI to both PICs
        outb(PIC2_COMMAND, PIC_EOI);
    }
    
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
    
    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = inb(port) | (1 << irq);
    outb(port, value);
}

void pic_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;
    
    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void pic_disable() {
    // Mask all interrupts
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}