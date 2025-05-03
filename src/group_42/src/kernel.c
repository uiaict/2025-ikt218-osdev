#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "libc/stdio.h"

#include <multiboot2.h>

#include "kernel/interrupts.h"
#include "kernel/pic.h"
#include "kernel/system.h"

#include "shell/shell.h"

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
  printf("Testing %s%c%c%d%c%d\n", "print", 'f', ' ', 6969, ' ', -420);

  print("Initialising PIC...\n");
  remap_pic();

  print("Initialising interrupts...\n");
  init_interrupts();

  for (int i = 0; i < 1000000; i++) {
    io_wait();
  }

  switch_to_protected_mode();
  print("Protected mode enabled.\n");

  asm volatile("sti");
  print("Interrupts enabled.\n");

  print("Initializing kernel memory...\n");
  init_kernel_memory(&end);

  print("Initializing paging...\n");
  init_paging();

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

  sleep_busy(1000);
  print("Testing sleep_busy...\n");

  sleep_interrupt(1000);
  print("Testing sleep_interrupt...\n");

  print("Initializing shell...\n");
  sleep_busy(1000);
  shell_init();

  while (true) {
  }

  return 0;
}