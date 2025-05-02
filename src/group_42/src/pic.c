#include "kernel/pic.h"
#include "kernel/system.h"

/*
To avoid conflicts with CPU exceptions, the PIC is typically remapped to start
at interrupt vector 0x20 (32) for IRQ0. After remapping:

IRQ0 (Timer) → Interrupt Vector 0x20 (32)
IRQ1 (Keyboard) → Interrupt Vector 0x21 (33)
IRQ2 → Interrupt Vector 0x22 (34)
And so on…
*/

void remap_pic() {
  // TODO: This needs a source

  outb(0x20, 0x11); // Start initialization of master PIC
  outb(0xA0, 0x11); // Start initialization of slave PIC
  outb(0x21, 0x20); // Remap master PIC to 0x20-0x27
  outb(0xA1, 0x28); // Remap slave PIC to 0x28-0x2F
  outb(0x21, 0x04); // Tell master PIC about the slave PIC
  outb(0xA1, 0x02); // Tell slave PIC its cascade identity
  outb(0x21, 0x01); // Set master PIC to 8086 mode
  outb(0xA1, 0x01); // Set slave PIC to 8086 mode
  outb(0x21, 0x0);  // Enable all IRQs on master PIC
  outb(0xA1, 0x0);  // Enable all IRQs on slave PIC

  // Enable only keyboard interrupt
  // outb(0x21, 0xFD);
  // outb(0xA1, 0xFF);
}

void send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(0xA0, 0x20); // Send EOI to slave PIC
  }
  outb(0x20, 0x20); // Send EOI to master PIC
}