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

  printf("Testing %s%c", "print", 'f');

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

  print("Initializing kernel memory...\n");
  init_kernel_memory(&end);

  print("Initializing paging...\n");
  init_paging();

  print("Printing memory layout...\n");
  print_memory_layout();

  print("Memory allocation test...\n");
  void *some_memory = malloc(12345);
  void *memory2 = malloc(54321);
  void *memory3 = malloc(13331);

  print("Freeing memory...\n");
  free(some_memory);
  free(memory2);
  free(memory3);

  print("Initializing PIT...\n");
  init_pit();

  print("Testing Pit...\n");

  sleep_busy(1000);
  print("Sleep busy succeded\n");

  sleep_interrupt(1000);
  print("Sleep interrupt succeded\n");

  print("Initializing shell...\n");
  sleep_busy(3000);
  shell_init();

  while (true) {
  }

  return 0;
}