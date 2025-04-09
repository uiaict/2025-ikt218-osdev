// Oppdatert idt.c med riktige IRQ-stubber
#include "../src/arch/i386/ISR.h"
#include "../src/arch/i386/IRQ.h"
#include "libc/string.h"
#include "../src/arch/i386/io.h"
#include "../src/arch/i386/IDT.h"
#include "../src/screen.h"
#include "libc/stddef.h"

struct int_handler int_handlers[IDT_ENTRIES];
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_ptr;

// Function to register an interrupt handler
void register_int_handler(int num, void (*handler)(void *data), void *data) {
  int_handlers[num].num = num;
  int_handlers[num].handler = handler;
  int_handlers[num].data = data;
}

void default_int_handler(void *data) {
  write_to_terminal("Default interrupt handler triggered", 8);
}

void int_handler(int num) {
  if (int_handlers[num].handler != NULL) {
    int_handlers[num].handler(int_handlers[num].data);
  } else {
    default_int_handler(NULL);
  }
}

void init_idt() {
  idt_ptr.base = (uint32_t)&idt;
  idt_ptr.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;

  for (int i = 0; i < IDT_ENTRIES; i++) {
    idt[i].base_low = 0x0000;
    idt[i].base_high = 0x0000;
    idt[i].segment = 0x08;
    idt[i].zero = 0x00;
    idt[i].flags = 0x8E;
    int_handlers[i].handler = NULL;

    asm volatile("sti"); // Enable interrupts

  }

  remap_irq();

  set_idt_gate(32, (uint32_t)isr_stub_32, 0x08, 0x8E);
  set_idt_gate(33, (uint32_t)isr_stub_33, 0x08, 0x8E);
  set_idt_gate(34, (uint32_t)isr_stub_34, 0x08, 0x8E);
  set_idt_gate(35, (uint32_t)isr_stub_35, 0x08, 0x8E);
  set_idt_gate(36, (uint32_t)isr_stub_36, 0x08, 0x8E);
  set_idt_gate(37, (uint32_t)isr_stub_37, 0x08, 0x8E);
  set_idt_gate(38, (uint32_t)isr_stub_38, 0x08, 0x8E);
  set_idt_gate(39, (uint32_t)isr_stub_39, 0x08, 0x8E);
  set_idt_gate(40, (uint32_t)isr_stub_40, 0x08, 0x8E);
  set_idt_gate(41, (uint32_t)isr_stub_41, 0x08, 0x8E);
  set_idt_gate(42, (uint32_t)isr_stub_42, 0x08, 0x8E);
  set_idt_gate(43, (uint32_t)isr_stub_43, 0x08, 0x8E);
  set_idt_gate(44, (uint32_t)isr_stub_44, 0x08, 0x8E);
  set_idt_gate(45, (uint32_t)isr_stub_45, 0x08, 0x8E);
  set_idt_gate(46, (uint32_t)isr_stub_46, 0x08, 0x8E);
  set_idt_gate(47, (uint32_t)isr_stub_47, 0x08, 0x8E);

  idt_load(&idt_ptr);
  asm volatile("sti");
}



void idt_load(struct idt_ptr *idt_ptr) {
  asm volatile("lidt %0" : : "m" (*idt_ptr));
}

void *IRQ_routines[16] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //these are the 16 routines that are associated with our interrupt request
}


void IRQ_handler_install (int irq, void (*handler)(struct interruptRegister *r )) {   //den bruker (interrupt register) <-(må lages) som en argument
  IRQ_routines[irq] = handlers:
}

void IRQ_hanlder_uninstall(int irq){
  IRQ_routines[irq] = 0
}

void IRQ_handler(struct InterruptRegister* register){                 //lag en struct interruptRegister
  void (*handler)(struct InterruptRegister* register):

  handler = IRQ_routines[register->int_no - 32]

} 