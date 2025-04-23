#include "system.h"
#include "gdt.h"
#include "print.h"

void cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void cursor_disable() {
  /*
asm("mov $0x01, %AH;"
    "mov $0x3F, %CH;"
    "int $0x10;");
    */
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void update_cursor(int x, int y) {
  uint16_t pos = y * SCREEN_WIDTH + x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void) {
  uint16_t pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((uint16_t)inb(0x3D5)) << 8;
  return pos;
}

void switch_to_protected_mode() {
  // Disable interrupts
  __asm__ volatile("cli");

  // Enable A20 line using Fast A20 Gate
  uint8_t a20_status = inb(0x92);
  outb(0x92, a20_status | 2);

  gdt_install();

  // Set PE (Protection Enable) bit in CR0
  __asm__ volatile("movl %%cr0, %%eax\n"
                   "orl $1, %%eax\n"
                   "movl %%eax, %%cr0"
                   :
                   :
                   : "eax");

  // Long jump to clear the pipeline and switch to 32-bit code
  __asm__ volatile("ljmp $0x08, $protected_mode_jump\n"
                   "protected_mode_jump:\n"
                   ".code32\n"
                   "movw $0x10, %%ax\n"
                   "movw %%ax, %%ds\n"
                   "movw %%ax, %%es\n"
                   "movw %%ax, %%fs\n"
                   "movw %%ax, %%gs\n"
                   "movw %%ax, %%ss"
                   :
                   :
                   : "eax");
}