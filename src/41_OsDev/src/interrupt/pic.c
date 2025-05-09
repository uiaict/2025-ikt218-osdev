// pic.c

#include <libc/stdint.h>
#include <driver/include/port_io.h>

////////////////////////////////////////
// PIC Ports and Command Constants
////////////////////////////////////////

#define PIC1         0x20
#define PIC2         0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

////////////////////////////////////////
// PIC Remapping
////////////////////////////////////////

// Remap PIC to new interrupt vector offsets
void pic_remap(int offset1, int offset2) {
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);  // Set vector offset for master
    outb(PIC2_DATA, offset2);  // Set vector offset for slave

    outb(PIC1_DATA, 4);        // Tell Master PIC there is a slave at IRQ2
    outb(PIC2_DATA, 2);        // Tell Slave PIC its cascade identity

    outb(PIC1_DATA, a1);       // Restore saved masks
    outb(PIC2_DATA, a2);
}

////////////////////////////////////////
// IRQ Setup
////////////////////////////////////////

// Initialize IRQ by remapping PIC
void init_irq(void) {
    pic_remap(0x20, 0x28);
}
