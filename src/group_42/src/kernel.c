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

  print("Initialising PIC...\n");
  remap_pic();

  print("Initialising interrupts...\n");
  init_interrupts();

  print("Waiting...\n");

  for (int i = 0; i < 1000000; i++) {
    io_wait();
  }

  print("Done waiting\n");
  print("Switching to protected mode...\n");
  switch_to_protected_mode();
  print("Protected mode switched.\n");

  print("Enabling interrupts...\n");
  asm volatile("sti");
  print("Interrupts enabled.\n");

  while (true) {
  }

  return 0;
}