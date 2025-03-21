#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x07

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

  // Null segment
  gdt_set_gate(0, 0, 0, 0, 0);
  // Code segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  // Data segment
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  // User mode code segment
  gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  // User mode data segment
  gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

  gdt_load(&gdt_ptr);
}

// Enkel VGA-tekstutskrift
void vga_print(const char *message) {
  volatile char *vga = (volatile char *)VGA_ADDRESS;
  while (*message) {
    *vga++ = *message++;
    *vga++ = WHITE_ON_BLACK;
  }
}
void int_to_hex(uint32_t num, char *buf) {
  const char *hex_chars = "0123456789ABCDEF";
  for (int i = 7; i >= 0; i--) {
    buf[i] = hex_chars[num & 0xF];
    num >>= 4;
  }
  buf[8] = '\0';
}

// Erstattet printf() med vga_print()
void check_gdt() {
  struct gdt_ptr gdtr;
  asm volatile("sgdt %0" : "=m"(gdtr));

  char buffer1[9];
  int_to_hex(gdtr.base, buffer1);

  char buffer2[9];

  int_to_hex(gdtr.limit, buffer2);
  vga_print("Hello world!");
}

int kernel_main();

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
  init_gdt();
  check_gdt();
  return kernel_main();
}
