#include "libc/stdint.h"
#include "libc/io.h"
#include "isr.h"
#include "libc/monitor.h"

// Array to store interrupt handlers
isr_t interrupt_handlers[256];

const char* interrupt_names[256] = {
    "Divide by Zero Exception",          // 0
    "Debug Exception",                   // 1
    "Non-Maskable Interrupt",            // 2
    "Breakpoint Exception",              // 3
    "Overflow Exception",                // 4
    "Bound Range Exceeded Exception",    // 5
    "Invalid Opcode Exception",          // 6
    "Device Not Available Exception",    // 7
    "Double Fault Exception",            // 8
    "Coprocessor Segment Overrun",       // 9
    "Invalid TSS Exception",             // 10
    "Segment Not Present",               // 11
    "Stack-Segment Fault",               // 12
    "General Protection Fault",          // 13
    "Page Fault",                        // 14
    "Reserved",                          // 15
    "x87 Floating-Point Exception",      // 16
    "Alignment Check Exception",         // 17
    "Machine Check Exception",           // 18
    "SIMD Floating-Point Exception",     // 19
    "Virtualization Exception",          // 20
    "Reserved",                          // 21
    "Reserved",                          // 22
    "Reserved",                          // 23
    "Reserved",                          // 24
    "Reserved",                          // 25
    "Reserved",                          // 26
    "Reserved",                          // 27
    "Reserved",                          // 28
    "Reserved",                          // 29
    "Reserved",                          // 30
    "Reserved",                          // 31
    "IRQ0 - Timer",                      // 32
    "IRQ1 - Keyboard",                   // 33
    "IRQ2 - Cascade",                    // 34
    "IRQ3 - COM2",                       // 35
    "IRQ4 - COM1",                       // 36
    "IRQ5 - LPT2",                       // 37
    "IRQ6 - Floppy Disk",                // 38
    "IRQ7 - LPT1",                       // 39
    "IRQ8 - CMOS Real-Time Clock",       // 40
    "IRQ9 - Free for peripherals",       // 41
    "IRQ10 - Free for peripherals",      // 42
    "IRQ11 - Free for peripherals",      // 43
    "IRQ12 - PS/2 Mouse",                // 44
    "IRQ13 - FPU",                       // 45
    "IRQ14 - Primary ATA Hard Disk",     // 46
    "IRQ15 - Secondary ATA Hard Disk",   // 47
    "Reserved",                          // 48-255
};

void isr_handler(registers_t regs) {
    monitor_write("Received interrupt: ");
    
    // Check if the interrupt number is within the range of defined names
    if (regs.int_no < 48) {
        monitor_write(interrupt_names[regs.int_no]); // Print the interrupt name
    } else {
        monitor_write("Unknown Interrupt"); // Handle undefined interrupts
    }
    
    monitor_put('\n');
}
//Function is called from isr.asm when an IRQ occurs
void irq_handler(registers_t regs){
    // If this interrupt involved the slave
   if (regs.int_no >= 40){
       outb(0xA0, 0x20); //Sends reset signal to slave PIC
   }

   outb(0x20, 0x20);     //Sends reset signal to master PIC

    // Call the interrupt handler if it exists
   if (interrupt_handlers[regs.int_no] != 0){
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

// Function to register an interrupt handler
void register_interrupt_handler(uint8_t n, isr_t handler){
  interrupt_handlers[n] = handler;
}