#include "interrupts.h"
#include "common.h"
#include "terminal.h"

// Handler for IRQ0 (timer)
void timer_handler(registers_t* regs, void* data) {
    // Implementation details
    static uint32_t tick = 0;
    tick++;
    
    // Maybe print a message every second (assuming 100Hz timer)
    if (tick % 100 == 0) {
        //terminal_writestring("Tick\n");
    }
}

// Initialize IRQ handlers
void init_irq() {
  // Initialize the irq_handlers array
  for (int i = 0; i < IRQ_COUNT; i++) {
      irq_handlers[i].data = NULL;
      irq_handlers[i].handler = NULL;
      irq_handlers[i].num = i;
  }

  // Write to video memory to confirm this function runs
  uint16_t* video_memory = (uint16_t*)0xB8000;
  video_memory[80] = (0x0F << 8) | 'P'; // Row 1, column 0
  video_memory[81] = (0x0F << 8) | 'I'; // Row 1, column 1
  video_memory[82] = (0x0F << 8) | 'C'; // Row 1, column 2

  // Initialize the PIC
  outb(0x20, 0x11);  // ICW1: Initialize master PIC
  outb(0xA0, 0x11);  // ICW1: Initialize slave PIC
  outb(0x21, 0x20);  // ICW2: Map master PIC interrupts to 0x20-0x27
  outb(0xA1, 0x28);  // ICW2: Map slave PIC interrupts to 0x28-0x2F
  outb(0x21, 0x04);  // ICW3: Tell master PIC about slave
  outb(0xA1, 0x02);  // ICW3: Tell slave PIC its identity
  outb(0x21, 0x01);  // ICW4: Set master PIC to 8086 mode
  outb(0xA1, 0x01);  // ICW4: Set slave PIC to 8086 mode

  // Unmask specific interrupts - only enable keyboard for testing
  outb(0x21, ~0x02);  // Enable only IRQ1 (keyboard) on master PIC
  outb(0xA1, 0xFF);   // Mask all interrupts on slave PIC

  // Mark PIC initialization complete
  video_memory[84] = (0x0F << 8) | 'O';
  video_memory[85] = (0x0F << 8) | 'K';

  // Enable interrupts
  asm volatile("sti");
}

// Register an IRQ handler
void register_irq_handler(int irq, isr_t handler, void* ctx) {
    irq_handlers[irq].handler = handler;
    irq_handlers[irq].data = ctx;
}

// The main IRQ handler
void irq_handler(registers_t regs) {
  // Direct video memory output
  uint16_t* video_memory = (uint16_t*)0xB8000;
  
  // If this is the keyboard IRQ
  if ((regs.int_no - 32) == 1) {
      // Write "KB!" at the top left corner of the screen
      video_memory[0] = (0x0F << 8) | 'K';
      video_memory[1] = (0x0F << 8) | 'B';
      video_memory[2] = (0x0F << 8) | '!';
  }
  
  // Send EOI signals
  if (regs.int_no >= 40) {
      outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
  
  // Call handler if registered
  uint32_t irq_no = regs.int_no - 32;
  if (irq_no < IRQ_COUNT && irq_handlers[irq_no].handler) {
      irq_handlers[irq_no].handler(&regs, irq_handlers[irq_no].data);
  } else if (int_handlers[regs.int_no].handler) {
      int_handlers[regs.int_no].handler(&regs, int_handlers[regs.int_no].data);
  }
}