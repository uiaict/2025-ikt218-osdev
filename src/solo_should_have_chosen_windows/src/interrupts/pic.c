#include "interrupts/pic.h"

#include "libc/io.h"
#include "libc/stdint.h"

/* In plain English: 
    Hey PICs, reset yourselves. Here’s your new range of interrupt numbers. Master, your slave is on IRQ2. Both of you: use the standard PC mode. Now go back to how you were blocking IRQs.
*/

void pic_remap(uint8_t offset1, uint8_t offset2) {
    uint8_t a1, a2;

    // Save masks so that the right IRQs are enabled/disabled
    __asm__ volatile ("inb %1, %0" : "=a"(a1) : "Nd"(0x21));
    __asm__ volatile ("inb %1, %0" : "=a"(a2) : "Nd"(0xA1));

    // Start the initialization sequence in cascade mode
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Set the offsets
    outb(0x21, offset1);
    outb(0xA1, offset2);

    // Tell the PICs about each other
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // Set the PICs to 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Restore the masks
    outb(0x21, a1);
    outb(0xA1, a2);
}