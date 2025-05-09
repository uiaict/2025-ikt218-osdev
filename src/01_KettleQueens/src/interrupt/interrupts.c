#include "printing/terminal.h"
#include "interrupt/interrupts.h"
#include "libc/stdint.h"
#include "util.h"

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};

const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
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

void isr_handler(struct InterruptRegisters* regs) {
   if (regs->int_no < 32) {
       // Handle CPU exceptions
       terminal_write("CPU Exception: ");
       terminal_write(exception_messages[regs->int_no]);
       terminal_write("\n");
       terminal_write("Exception! System halted.\n");
       for ( ; ; )
       {
       }
   } 
}

void *irq_routines [16] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters* regs)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters* regs) {
    void (*handler)(struct InterruptRegisters* regs);
    handler = irq_routines[regs->int_no - 32];
    if (handler) {
        handler(regs);
    }

    if (regs->int_no >= 40) {
        outb(0xA0, 0x20); 
    }
    
    outb(0x20, 0x20);
}
