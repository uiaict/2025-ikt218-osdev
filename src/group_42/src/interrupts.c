#include "interrupts.h"
#include "idt.h"
#include "keyboard.h"
#include "print.h"
#include "system.h"

void default_interrupt_handler() {
  char *video_memory = (char *)0xB8000;
  video_memory[0] = 'U';  // Write to the top-left corner of the screen
  video_memory[1] = 0x07; // White text on black background

  // Send EOI to the Master PIC
  outb(0x20, 0x20);
}

// IRQ 7 is apparently a spurious interrupt according to
// https://wiki.osdev.org/Interrupts
// Therefore this function is just for debugging to see if it triggers.

void spurious_interrupt_handler() {
  char *video_memory = (char *)0xB8000;
  video_memory[0] = 'S';  // Write to the top left of the screen.
  video_memory[1] = 0x07; // White text on black background
}

void init_interrupts() {
  // FIll the IDT with default handlers
  for (int i = 0; i < IDT_ENTRIES; i++) {
    set_idt_entry(i, (uint32_t)default_interrupt_handler);
  }
  set_idt_entry(0x21, (uint32_t)keyboard_handler); // IRQ1 for keyboard
  set_idt_entry(0x27, (uint32_t)spurious_interrupt_handler); // Spurious IRQ7
  load_idt();
}