#include "interrupts/pic.h"
#include "interrupts/io.h"

void pic_remap(int offset1, int offset2) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11); // init
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, offset1); // remap offset
    outb(PIC2_DATA, offset2);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, mask1); // restore saved masks
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) outb(0xA0, 0x20); // slave PIC
    outb(0x20, 0x20);               // master PIC
}
