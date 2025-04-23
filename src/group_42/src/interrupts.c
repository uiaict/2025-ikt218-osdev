#include "interrupts.h"
#include "idt.h"
#include "print.h"
#include "system.h"

void default_interrupt_handler() {

  /*
  // Debug code to see if the default interrupt handler gets triggered.
  volatile char *video_memory = (volatile char *)0xB8000;
  video_memory[0] = 'U';
  video_memory[1] = VIDEO_WHITE;
  */

  outb(0x20, 0x20);
}

void spurious_interrupt_handler() {
  /*
  // Debug code to see if the spurious interrupt handler gets triggered.

  volatile char *video_memory = (volatile char *)0xB8000;
  video_memory[2] = 'S';
  video_memory[3] = VIDEO_WHITE;
  */

  outb(0xA0, 0x20);
  outb(0x20, 0x20);
}

void keyboard_handler() {
  volatile char *video_memory = (volatile char *)0xB8000;

  volatile uint8_t scancode = inb(0x60);

  video_memory[0] = '0' + (scancode >> 4); // Display high nibble
  video_memory[1] = VIDEO_WHITE;

  outb(0x20, 0x20);
}

extern void default_interrupt_handler_wrapper(void);
extern void spurious_interrupt_handler_wrapper(void);
extern void keyboard_handler_wrapper(void);

void init_interrupts() {
  // Point IDT entries to the assembly wrappers
  for (int i = 0; i < IDT_ENTRIES; i++) {
    set_idt_entry(i, (uint32_t)default_interrupt_handler_wrapper);
  }

  set_idt_entry(IRQ1, (uint32_t)keyboard_handler_wrapper);
  set_idt_entry(IRQ7, (uint32_t)spurious_interrupt_handler_wrapper);

  load_idt();
}