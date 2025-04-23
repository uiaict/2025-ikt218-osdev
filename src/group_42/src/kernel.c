#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#include "interrupts.h"
#include "keyboard.h"
#include "pic.h"
#include "print.h"
#include "system.h"

struct multiboot_info {
  uint32_t size;
  uint32_t reserved;
  struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
  cursor_disable();
  print("Just booted. Hello!\n");

  VideoColour colour = 1;

  print("Initialising PIC...\n");
  remap_pic();

  print("Initialising interrupts...\n");
  init_interrupts();

  print("Setting up PS/2 keyboard\n");

  // init_ps2();

  print("Set up PS/2 keybaord done\n");
  print("Waiting...\n");

  for (int i = 0; i < 1000000; i++) {
    io_wait();
  }

  print("Done waiting\n");
  print("Switching to protected mode...\n");
  switch_to_protected_mode();
  print("Protected mode switched.\n");

  asm volatile("sti");
  print("Interrupts enabled.\n");

  while (true) {
    printc("Test\n", colour);
    for (int i = 0; i < 1000000; i++)
      io_wait();
    colour++;
    if (colour >= VIDEO_WHITE)
      colour = 1;
  }

  return 0;
}