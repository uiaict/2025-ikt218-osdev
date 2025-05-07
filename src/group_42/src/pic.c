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
  // 1) start init sequence (cascade mode)
  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  // 2) set vector offset
  outb(0x21, 0x20); // master: IRQ0–7 → int 0x20–0x27
  outb(0xA1, 0x28); // slave : IRQ8–15 → int 0x28–0x2F

  // 3) tell PICs how they’re wired
  outb(0x21, 0x04); // master has a slave on IR2
  outb(0xA1, 0x02); // slave cascades via IRQ2

  // 4) 8086 mode
  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  // 5) restore masks—but here we explicitly mask _all_ except IRQ1
  //    bit = 1 means “masked off,” so 0xFD = 11111101b → only bit1 is 0 (unmasked)
  outb(0x21, 0xFC); // master: mask everything except IRQ1 (keyboard)
  outb(0xA1, 0xFF); // slave : mask all
}

void send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(0xA0, 0x20); // Send EOI to slave PIC
  }
  outb(0x20, 0x20); // Send EOI to master PIC
}