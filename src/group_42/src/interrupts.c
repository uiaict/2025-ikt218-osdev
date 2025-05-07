#include "kernel/interrupts.h"
#include "kernel/idt.h"
#include "kernel/keyboard.h"
#include "kernel/pit.h"
#include "kernel/system.h"
#include "kernel/keyboard.h"
#include "libc/stdio.h"
#include "shell/command.h"
#include "shell/shell.h"

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
  volatile uint8_t scancode = inb(0x60);

  if (!(scancode & 0x80)) { // Key press
    if (scancode < SCANCODE_MAX) {
      char ascii = scancode_to_ascii[scancode];
      input_route_keystroke(ascii);
    }
  }
  outb(0x20, 0x20);
}

extern void default_interrupt_handler_wrapper(void);
extern void spurious_interrupt_handler_wrapper(void);
extern void keyboard_handler_wrapper(void);
extern void pit_interrupt_handler_wrapper(void);

void init_interrupts() {
  remap_pic();
  for (int i = 0; i < IDT_ENTRIES; i++) {
      set_idt_entry(i, (uint32_t)default_interrupt_handler_wrapper);
  }
  set_idt_entry(IRQ0, (uint32_t)pit_interrupt_handler_wrapper);
  set_idt_entry(IRQ1, (uint32_t)keyboard_handler_wrapper);
  set_idt_entry(IRQ7, (uint32_t)spurious_interrupt_handler_wrapper);

  input_init();
  load_idt();
}