#include "gdt.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>

extern void isr0();
extern void isr1();
extern void isr14();
extern void irq0();
extern void irq1();

#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define PIT_FREQUENCY 1193180

static uint32_t ticks = 0;

typedef struct registers {
  uint32_t gs, fs, es, ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags;
} registers_t;

void pit_irq_handler(registers_t *r, void *ctx) { printf("Tick\n"); }

void init_pit(uint32_t frequency) {
  uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

  register_irq_handler(0, pit_irq_handler, NULL);

  outb(PIT_COMMAND_PORT, 0x36);
  outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
  outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

#define VGA_ADDRESS 0xB8000
volatile char *vga = (volatile char *)VGA_ADDRESS;
int cursor = 0;

void printf(const char *message) {
  for (int i = 0; message[i] != '\0'; i++) {
    vga[(cursor + i) * 2] = message[i];
    vga[(cursor + i) * 2 + 1] = 0x07;
  }
  cursor += 80;
  if (cursor >= 80 * 25)
    cursor = 0;
}

void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

void remap_pic() {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x00);
  outb(0xA1, 0x00);
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

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
  idt[n].base_low = handler & 0xFFFF;
  idt[n].sel = sel;
  idt[n].always0 = 0;
  idt[n].flags = flags;
  idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void lidt(void *idtp) { asm volatile("lidt (%0)" : : "r"(idtp)); }

void init_idt() {
  idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
  idtp.base = (uint32_t)&idt;

  idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);

  lidt(&idtp);
}

typedef void (*isr_t)(registers_t *, void *);

#define IRQ_COUNT 256
struct int_handler_t {
  isr_t handler;
  void *data;
  int num;
};

struct int_handler_t irq_handlers[IRQ_COUNT];

void register_irq_handler(int irq, isr_t handler, void *ctx) {
  irq_handlers[irq + 32].handler = handler;
  irq_handlers[irq + 32].data = ctx;
}

void irq_handler(registers_t *regs) {
  if (regs->int_no == 32) {
    printf("IRQ0 fired\n");
  }

  if (regs->int_no >= 40)
    outb(0xA0, 0x20);
  outb(0x20, 0x20);
}

void isr_handler(registers_t *regs) {
  printf("ISR triggered: ");
  if (regs->int_no == 0) {
    printf("Divide by zero\n");
  } else if (regs->int_no == 14) {
    printf("Page fault\n");
  } else {
    printf("Unhandled interrupt number\n");
  }

  while (1)
    asm volatile("hlt");
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
  init_gdt();
  remap_pic();
  init_idt();
  init_pit(100);

  asm volatile("sti");
}
