#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#include "interrupts.h"
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

  print("Setting up PS/2 keyboard\n");

  outb(0x64, 0xAE); // Enable first PS/2 port
  while (inb(0x64) & 0x02)
    ;               // Wait until input buffer is empty
  outb(0x60, 0xF4); // Enable keyboard scanning

  print("Initialising PIC...\n");
  remap_pic();

  print("Initialising interrupts...\n");
  init_interrupts();
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