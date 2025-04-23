#include "keyboard.h"
#include "print.h"
#include "system.h"

void keyboard_handler() {
  char *video_memory = (char *)0xB8000;
  video_memory[4] = 'K'; // Write to the to of the screen FOR DEBUGGING
  video_memory[5] = 0x07;

  uint8_t scancode = inb(0x60); // Read scancode from keyboard port

  // Send EOI to the Master PIC
  outb(0x20, 0x20);
}