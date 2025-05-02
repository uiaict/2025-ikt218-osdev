#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#include "interrupts.h"
#include "pic.h"
#include "print.h"
#include "system.h"

#include "apps/shell.h"

#include "kernel/memory.h"
#include "kernel/pit.h"

extern uint32_t end; // This is defined in arch/i386/linker.ld

struct multiboot_info {
  uint32_t size;
  uint32_t reserved;
  struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
  cursor_disable();
  print("Just booted. Hello!\n");

  // Print a hello world message with printf.
  printf("%s%c%c%c%c%c%c%c%c\n", "Hello", ' ', 'W', 'O', 'R', 'L', 'D');

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

  // Initialize the kernel's memory manager using the end address of the kernel.
  init_kernel_memory(&end); // <------ THIS IS PART OF THE ASSIGNMENT

  // Initialize paging for memory management.
  init_paging(); // <------ THIS IS PART OF THE ASSIGNMENT

  // Print memory information.
  print_memory_layout(); // <------ THIS IS PART OF THE ASSIGNMENT

  // Initialize PIT
  //init_pit(); // <------ THIS IS PART OF THE ASSIGNMENT

  shell_init();
  while (true) {
  }

  return 0;
}