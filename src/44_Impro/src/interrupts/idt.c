#include "idt.h"
#include "libc/stdint.h"
#include "util.h"
#include <libc/stddef.h>
#include "vga.h"
#include "parser.h"
#include "libc/stdio.h"

//struct int_handler int_handlers[IDT_ENTRIES];
struct idt_entry idt_entries[IDT_ENTRIES];
struct idt_ptr idt_ptr;


const char scancode_ascii[] = {
  0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
  '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
  0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
  0, '\\','z','x','c','v','b','n','m',',','.','/',
  0, '*', 0, ' ', 0
};



void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].flags = flags | 0x60;
}


const char* exception_msg[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment not present",
  "Stack fault",
  "General protection fault",
  "Page fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Fault",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};



void isr_handler(InterruptRegisters* regs) {
  
    if (regs->int_num == 33) {
        printf("Software INT 33 triggered\n\r");
        return;
    }

    if (regs->int_num == 14) {
        uint32_t fault_addr;
        asm volatile("mov %%cr2, %0" : "=r"(fault_addr));
        printf("Page Fault Exception\n\r");
        printf("Page Fault Exception\n\r");
        printf("Faulting address: 0x");
        print_hex(fault_addr);
        printf("\n\r");

        printf("Error code: ");
        print_hex(regs->err_code);
        printf("\n\r");

        if (!(regs->err_code & 0x1)) printf(" - Not present\n\r");
        if (regs->err_code & 0x2) printf(" - Write\n\r");
        if (regs->err_code & 0x4) printf(" - User-mode\n\r");
        if (regs->err_code & 0x8) printf(" - Reserved bit\n\r");
        if (regs->err_code & 0x10) printf(" - Instruction fetch\n\r");

        for (;;); 
    }

    if (regs->int_num < 32) {
        printf(exception_msg[regs->int_num]);
        printf("\n\r!Exception!\n\r");
        for (;;);
    }
}

void keyboard_callback(struct InterruptRegisters* r) {
  uint8_t scancode = inPortB(0x60);

  if (scancode > 57) return;

  char c = scancode_ascii[scancode];
  if (c && c != '\n') {
      if (input_index < INPUT_BUFFER_SIZE - 1) {
        input_buffer[input_index++] = c;
        input_buffer[input_index] = '\0';
        char str[2] = {c, 0};
        printf(str);
      }
  }else if (c == '\n') {
      printf("\n\r");

      process_command(input_buffer);

      input_index = 0;
      input_buffer[0] = '\0';
  }
}

void *irq_routines[16] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r)){
  irq_routines[irq] = handler;
}



void irq_uninstall_handler(int irq){
  irq_routines[irq] = 0;
}
void irq_handler(struct InterruptRegisters* regs) {
  void (*handler)(struct InterruptRegisters *regs);
  handler = irq_routines[regs->int_num - 32];
  if (handler) {
      handler(regs);
  }
  if (regs->int_num >= 40) {
      outPortB(0xA0, 0x20);
  }
  outPortB(0x20, 0x20);
}


extern void idt_flush(uint32_t idt_ptr);

void init_idt() {
  idt_ptr.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
  idt_ptr.base = (uint32_t) &idt_entries;


  
  for (int i = 0; i < IDT_ENTRIES; i++) {
    idt_set_gate(i,0,0,0);
		//int_handlers[i].handler = NULL;
  }
  irq_install_handler(1, keyboard_callback);

outPortB(0x20, 0x11);
outPortB(0xA0, 0x11);

outPortB(0x21, 0x20);
outPortB(0xA1, 0x28);

outPortB(0x21, 0x04);
outPortB(0xA1, 0x02);

outPortB(0x21, 0x01);
outPortB(0xA1, 0x01);

outPortB(0x21, 0x0);
outPortB(0xA1, 0x0);




  idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
  idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
  idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

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


  idt_set_gate(128, (uint32_t)isr128, 0x08, 0x8E);
  idt_set_gate(177, (uint32_t)isr177, 0x08, 0x8E);
  
  idt_flush((uint32_t)&idt_ptr);

}