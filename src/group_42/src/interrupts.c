#include "interrupts.h"
#include "idt.h"
#include "print.h"
#include "system.h"

void default_interrupt_handler() {
  asm volatile("cli");

  volatile char *video_memory = (volatile char *)0xB8000;
  video_memory[0] = 'U';
  video_memory[1] = 0x07;

  outb(0x20, 0x20);
  asm volatile("sti");
}

void spurious_interrupt_handler() {
  asm volatile("cli");
  char *video_memory = (char *)0xB8000;
  video_memory[2] = 'S';
  video_memory[3] = 0x07;

  outb(0xA0, 0x20);
  outb(0x20, 0x20);
  asm volatile("sti");
}

void keyboard_handler() {
  asm volatile("cli");
  char *video_memory = (char *)0xB8000;
  video_memory[0] = 'K';
  video_memory[1] = 0x07;

  volatile uint8_t scancode = inb(0x60);

  outb(0x20, 0x20);
  asm volatile("sti");
}

void init_interrupts() {
  // Point IDT entries to the assembly wrappers
  for (int i = 0; i < IDT_ENTRIES; i++) {
    set_idt_entry(i, (uint32_t)default_interrupt_handler);
  }

  set_idt_entry(0x21, (uint32_t)keyboard_handler);
  set_idt_entry(0x27, (uint32_t)spurious_interrupt_handler);

  load_idt();

  asm volatile("sti");
}