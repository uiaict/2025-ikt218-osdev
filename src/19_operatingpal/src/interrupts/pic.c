#include "interrupts/pic.h"
#include "interrupts/io.h"

// Remaps the PIC to avoid conflicts with CPU exceptions
void pic_remap(int offset1, int offset2) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11); // init
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, offset1); // set new vector offset
    outb(PIC2_DATA, offset2);
    outb(PIC1_DATA, 0x04);    // tell PIC1 about PIC2
    outb(PIC2_DATA, 0x02);    // tell PIC2 its cascade identity
    outb(PIC1_DATA, 0x01);    // set 8086 mode
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, mask1);   // restore saved masks
    outb(PIC2_DATA, mask2);
}

// Sends End-Of-Interrupt (EOI) to the appropriate PIC
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) outb(0xA0, 0x20); // slave PIC
    outb(0x20, 0x20);               // master PIC
}
