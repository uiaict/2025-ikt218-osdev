#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include "printf.h"
#include <multiboot2.h>

extern void isr0();
extern void isr1();
extern void isr14();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define PIT_FREQUENCY 1193180

void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

static const char scancode_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', // 0x00 - 0x09
    '9', '0', '+', '\\', '\b',                     // 0x0A - 0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^',
    '\n',                                                  // 0x0F - 0x1C
    0,                                                     // ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\'',     // 0x1D -
                                                           // 0x28
    0,                                                     // left shift
    '<', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', // 0x2C - 0x35
    0,                                                     // right shift
    '*',
    0,   // alt
    ' ', // space
    0,   // caps lock
         // resten kan være null
};

typedef struct registers {
  uint32_t gs, fs, es, ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags;
} registers_t;

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
  if (regs->int_no == 33) {
    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80)) { // Key press
      if (scancode < 128) { // Scancode under 128 for key press to not go out of bounds
          char ascii = scancode_ascii[scancode];

          if (scancode == 14) { // Backspace
            if (cursor > 0) {
              cursor--;
              vga[cursor * 2] = ' ';
              vga[cursor * 2 + 1] = 0x07;
            }
          } else if (ascii != 0) { 
            if (ascii >= ' ' && ascii <= '~') {
            char msg[2] = {ascii, '\0'};
            printf(msg);
             }
          }
      }
    }
  }
  if (regs->int_no >= 40) {
    outb(0xA0, 0x20); // Send to slave PIC
  }
  outb(0x20, 0x20); // Send to master PIC
}

static uint32_t ticks = 0;

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

void pit_irq_handler(registers_t *r, void *ctx) {};

void init_pit(uint32_t frequency) {
  uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

  register_irq_handler(0, pit_irq_handler, NULL);

  outb(PIT_COMMAND_PORT, 0x36);
  outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
  outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
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

void lidt(void *idtp) { asm volatile("lidt (%0)" : : "r"(idtp)); }

void init_idt() {
  idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
  idtp.base = (uint32_t)&idt;

  idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
  idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
  idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
  idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
  idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
  idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
  idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
  idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
  idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
  idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
  idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
  idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
  idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
  idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
  idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

  lidt(&idtp);

  remap_pic();
  register_irq_handler(1, irq_handler, NULL);
}
