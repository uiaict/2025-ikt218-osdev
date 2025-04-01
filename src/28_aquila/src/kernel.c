#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#define VGA_ADDRESS 0xB8000

extern void isr0();
extern void isr1();
extern void isr2();

struct multiboot_info {
  uint32_t size;
  uint32_t reserved;
  struct multiboot_tag *first;
};

struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[5];
struct gdt_ptr gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran) {
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = (limit >> 16) & 0x0F;
  gdt[num].granularity |= gran & 0xF0;
  gdt[num].access = access;
}

void gdt_load(struct gdt_ptr *gdt_ptr) {
  asm volatile("lgdt %0" : : "m"(*gdt_ptr));

  asm volatile("mov $0x10, %%ax\n"
               "mov %%ax, %%ds\n"
               "mov %%ax, %%es\n"
               "mov %%ax, %%fs\n"
               "mov %%ax, %%gs\n"
               "mov %%ax, %%ss\n"
               :
               :
               : "ax");
}

void init_gdt() {

  gdt_ptr.limit = sizeof(struct gdt_entry) * 5 - 1;
  gdt_ptr.base = (uint32_t)&gdt;

  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
  gdt_load(&gdt_ptr);
}

void printf(const char *message) {
  volatile char *vga = (volatile char *)VGA_ADDRESS;
  for (int i = 0; message[i] != '\0'; i++) {
    vga[i * 2] = message[i];
  }
}

struct idt_entry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void set_idt_gate(int n, uint32_t handler_addr) {
  idt[n].base_low = handler_addr & 0xFFFF;
  idt[n].sel = 0x08;
  idt[n].always0 = 0;
  idt[n].flags = 0x8E;
  idt[n].base_high = (handler_addr >> 16) & 0xFFFF;
}

void init_idt() {
  idtp.limit = sizeof(struct idt_entry) * 256 - 1;
  idtp.base = (uint32_t)&idt;

  set_idt_gate(0, (uint32_t)isr0);
  set_idt_gate(1, (uint32_t)isr1);
  set_idt_gate(2, (uint32_t)isr2);

  lidt(&idtp);
}

void lidt(void *idtp) { asm volatile("lidt (%0)" : : "r"(idtp)); }

void isr_handler() {
  printf("Interrupt triggered!\n");
  while (1)
    ;
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
  init_gdt();
  init_idt();
  printf("Testing ISR...\n");
  asm volatile("int $0x0");

  return 0;
}
