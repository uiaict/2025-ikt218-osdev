#include "libc/stddef.h" // For NULL
#include "../src/arch/i386/io.h"
//#include "../src/screen.h"
//#include "ISR.h"
#include "../src/arch/i386/print.h"
#include "../src/arch/i386/IRQ.h"

#define IRQ0 32 // system timer. PIC type: Master PIC. PIC = (Programmable Interrupt Controller)
#define IRQ1 33 // keyboard. PIC type: Master PIC
#define IRQ2 34 // Cascaded to Slave PIC (Bridge). PIC type: Master PIC
#define IRQ3 35 // Serial Port (COM2). PIC type: Master PIC
#define IRQ4 36 // Serial Port (COM1). PIC type: Master PIC
#define IRQ5 37 // Sound Card, LPT2. PIC type: Master PIC
#define IRQ6 38 // Floppy Disk Controller. PIC type: Master PIC
#define IRQ7 39 // 	Parallel Port (LPT1). PIC type: Master PIC
#define IRQ8 40 // Real-time Clock (RTC): Slave PIC
#define IRQ9 41 // Redirected to IRQ2. PIC type: Slave PIC
#define IRQ10 42 // General Purpose (Unused). PIC type: Slave PIC
#define IRQ11 43 // General Purpose (Unused). PIC type: Slave PIC
#define IRQ12 44 // PS/2 Mouse. PIC type: Slave PIC
#define IRQ13 45 // Floating-Point Unit (FPU). PIC type: Slave PIC
#define IRQ14 46 // Primary Hard Disk Controller. PIC type: Slave PIC
#define IRQ15 47 // 	Secondary Hard Disk Controller. PIC type: Slave PIC
#define IRQ_COUNT 16 // Just a counter 

// Here we store the IRQ handlers in an array
void (*irq_handlers[IRQ_COUNT])(void);

// Initialization of irq handlers happens here
void init_irq() {for (int i = 0; i < IRQ_COUNT; i++) {irq_handlers[i] = NULL;}}

// Here we register new irq handleler
void register_irq_handler(int irq, void (*handler)(void)){irq_handlers[irq] = handler;}


// Here we define the function that gets called when an IRQ occurs
void irq_handler(int irq) {
  if (irq_handlers[irq] != NULL){
    irq_handlers[irq]();
  }
  if (irq >= 8) {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
}























