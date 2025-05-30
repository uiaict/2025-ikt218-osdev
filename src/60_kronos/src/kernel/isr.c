#include "kernel/isr.h"
#include "kernel/idt.h"
#include "libc/stdio.h"
#include "sys/io.h"

// Array to hold custom handler functions
isr_handler_t interrupt_handlers[256];

void isrs_install() {
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
    
    printf("ISRs installed successfully\n");
}

void register_interrupt_handler(uint8_t n, isr_handler_t handler) {
    interrupt_handlers[n] = handler;
}

// Descriptive names for CPU exceptions
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
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
    "Alignment Check",
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
    "Reserved"
};

// Main ISR handler function called from ASM stub
void isr_handler(registers_t regs) {
    if (regs.int_no < 32) {
        // Enhanced error reporting with more context
        printf("EXCEPTION: %s (Error code: %d)\n", exception_messages[regs.int_no], regs.err_code);
        printf("At EIP: 0x%x, CS: 0x%x, EFLAGS: 0x%x\n", regs.eip, regs.cs, regs.eflags);
        
        // Critical exceptions that we cannot recover from
        if (regs.int_no == 0 || regs.int_no == 8 || regs.int_no == 13 || regs.int_no == 14) {
            printf("CRITICAL: System halted due to unrecoverable exception\n");
            for(;;) { 
                asm volatile("cli; hlt"); // Halt the CPU
            }
        }
    }
    
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_handler_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void irq_install() {
    // Remap the PIC
    // ICW1: start initialization sequence (in cascade mode)
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // ICW2: define PIC vectors
    outb(0x21, 0x20); // IRQ 0-7 -> interrupts 32-39
    outb(0xA1, 0x28); // IRQ 8-15 -> interrupts 40-47
    
    // ICW3: continue initialization sequence
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    
    // ICW4: have the PICs use 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // Mask interrupts (0 = enable, 1 = disable)
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    
    // Install the IRQs in the IDT
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
    
    printf("IRQs installed successfully\n");
}

void irq_handler(registers_t regs) {
    // Send an EOI (End of Interrupt) to the PICs
    if (regs.int_no >= 40) {
        // Send to slave PIC
        outb(0xA0, 0x20);
    }
    // Send to master PIC
    outb(0x20, 0x20);
    
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_handler_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    } else {
        if (regs.int_no - 32 != 0) {
            printf("Received IRQ %d\n", regs.int_no - 32);
        }
    }
}

void print_interrupts(registers_t regs) {
    printf("Custom handler for interrupt %d\n", regs.int_no);
}