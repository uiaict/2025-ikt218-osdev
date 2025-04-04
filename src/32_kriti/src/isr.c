// isr.c
#include "isr.h"
#include "idt.h"
#include <kprint.h>

// Array of interrupt handlers
static interrupt_handler_t interrupt_handlers[256] = {0};

// C function that gets called from the assembly stubs
void isr_handler(uint8_t interrupt_num) {

    /*kprint("Received interrupt: 0x");
    kprint_hex(interrupt_num);
    kprint("\n");*/
    // Call the registered handler if it exists
    if (interrupt_handlers[interrupt_num] != 0) {
        interrupt_handlers[interrupt_num](interrupt_num);
    } else {
        kprint("Unhandled interrupt: 0x");
        kprint_hex(interrupt_num);
        kprint("\n");
    }
    
    // Send EOI to PIC for hardware interrupts
    if (interrupt_num >= 32 && interrupt_num < 48) {
        // Send EOI to slave PIC if needed
        if (interrupt_num >= 40) {
            outb(0xA0, 0x20);
        }
        // Send EOI to master PIC
        outb(0x20, 0x20);
    }
}

// Register a handler for an interrupt
void register_interrupt_handler(uint8_t interrupt_num, interrupt_handler_t handler) {
    interrupt_handlers[interrupt_num] = handler;
    kprint("Registered handler for interrupt 0x");
    kprint_hex(interrupt_num);
    kprint("\n");
}

// I/O port functions
void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

// Define each ISR in assembly
#define DEFINE_ISR(num) \
    __asm__( \
        ".global isr" #num "\n" \
        "isr" #num ":\n" \
        "   cli\n" \
        "   pushl $0\n" \
        "   pushl $" #num "\n" \
        "   jmp isr_common\n" \
    );

#define DEFINE_ISR_ERR(num) \
    __asm__( \
        ".global isr" #num "\n" \
        "isr" #num ":\n" \
        "   cli\n" \
        "   pushl $" #num "\n" \
        "   jmp isr_common\n" \
    );

// Define the common ISR handler in assembly
__asm__(
    "isr_common:\n"
    "   # Save registers\n"
    "   pusha\n"
    "   pushl %ds\n"
    "   pushl %es\n"
    "   pushl %fs\n"
    "   pushl %gs\n"
    
    "   # Load kernel data segment\n"
    "   movw $0x10, %ax\n"
    "   movw %ax, %ds\n"
    "   movw %ax, %es\n"
    "   movw %ax, %fs\n"
    "   movw %ax, %gs\n"
    
    "   # Call C handler - pass interrupt number as argument\n"
    "   movl 48(%esp), %eax\n"  
    "   pushl %eax\n"
    "   call isr_handler\n"
    "   addl $4, %esp\n"
    
    "   # Restore registers\n"
    "   popl %gs\n"
    "   popl %fs\n"
    "   popl %es\n"
    "   popl %ds\n"
    "   popa\n"
    
    "   # Clean up error code and interrupt number\n"
    "   addl $8, %esp\n"
    
    "   # Return from interrupt\n"
    "   sti\n"
    "   iret\n"
);

// Define ISR functions for exceptions (0-19)
DEFINE_ISR(0)    // Divide by Zero
DEFINE_ISR(1)    // Debug
DEFINE_ISR(2)    // NMI
DEFINE_ISR(3)    // Breakpoint
DEFINE_ISR(4)    // Overflow
DEFINE_ISR(5)    // Bound Range Exceeded
DEFINE_ISR(6)    // Invalid Opcode
DEFINE_ISR(7)    // Device Not Available
DEFINE_ISR_ERR(8)    // Double Fault (has error code)
DEFINE_ISR(9)    // Coprocessor Segment Overrun
DEFINE_ISR_ERR(10)   // Invalid TSS (has error code)
DEFINE_ISR_ERR(11)   // Segment Not Present (has error code)
DEFINE_ISR_ERR(12)   // Stack-Segment Fault (has error code)
DEFINE_ISR_ERR(13)   // General Protection Fault (has error code)
DEFINE_ISR_ERR(14)   // Page Fault (has error code)
DEFINE_ISR(15)   // Reserved
DEFINE_ISR(16)   // x87 FPU Error
DEFINE_ISR_ERR(17)   // Alignment Check (has error code)
DEFINE_ISR(18)   // Machine Check
DEFINE_ISR(19)   // SIMD Floating-Point Exception
DEFINE_ISR(20)   // Reserved
DEFINE_ISR(21)   // Reserved
DEFINE_ISR(22)   // Reserved
DEFINE_ISR(23)   // Reserved
DEFINE_ISR(24)   // Reserved
DEFINE_ISR(25)   // Reserved
DEFINE_ISR(26)   // Reserved
DEFINE_ISR(27)   // Reserved
DEFINE_ISR(28)   // Reserved
DEFINE_ISR(29)   // Reserved
DEFINE_ISR(30)   // Reserved
DEFINE_ISR(31)   // Reserved

// Define ISR functions for IRQs (32-47)
DEFINE_ISR(32)   // Timer (IRQ0)
DEFINE_ISR(33)   // Keyboard (IRQ1)
DEFINE_ISR(34)   // Cascade for PIC2 (IRQ2)
DEFINE_ISR(35)   // COM2 (IRQ3)
DEFINE_ISR(36)   // COM1 (IRQ4)
DEFINE_ISR(37)   // LPT2 (IRQ5)
DEFINE_ISR(38)   // Floppy Disk (IRQ6)
DEFINE_ISR(39)   // LPT1 (IRQ7)
DEFINE_ISR(40)   // Real-Time Clock (IRQ8)
DEFINE_ISR(41)   // Free for peripherals (IRQ9)
DEFINE_ISR(42)   // Free for peripherals (IRQ10)
DEFINE_ISR(43)   // Free for peripherals (IRQ11)
DEFINE_ISR(44)   // PS/2 Mouse (IRQ12)
DEFINE_ISR(45)   // FPU (IRQ13)
DEFINE_ISR(46)   // Primary ATA Hard Disk (IRQ14)
DEFINE_ISR(47)   // Secondary ATA Hard Disk (IRQ15)

// Handler for NMI interrupts (INT 0x2)
void nmi_handler(uint8_t interrupt_num) {
    kprint("Non-maskable interrupt (NMI) occurred!\n");
    // No need to send EOI for CPU exceptions
}

// Handler for breakpoint interrupts (INT 0x3)
void breakpoint_handler(uint8_t interrupt_num) {
    kprint("Breakpoint interrupt occurred!\n");
    // No need to send EOI for CPU exceptions
}

// Handler for general protection faults (INT 0x13/0xD)
void gpf_handler(uint8_t interrupt_num) {
    kprint("General Protection Fault (GPF) occurred!\n");
    // In a production kernel, you might want to print more diagnostic info
}

// Handler for FPU exceptions (INT 0x10)
void fpu_handler(uint8_t interrupt_num) {
    kprint("FPU Exception occurred!\n");
}



// Initialize the ISR system
void isr_init(void) {
    // Register exception handlers (0-31)
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
    
    // Register IRQ handlers (32-47)
    idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)isr34, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)isr35, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)isr36, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)isr37, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)isr38, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)isr39, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)isr40, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)isr41, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)isr42, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)isr43, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)isr44, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)isr45, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)isr46, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)isr47, 0x08, 0x8E);
    
    // Register custom handlers for specific interrupts
    register_interrupt_handler(2, nmi_handler);            // NMI
    register_interrupt_handler(3, breakpoint_handler);     // Breakpoint
    register_interrupt_handler(13, gpf_handler);           // GPF
    register_interrupt_handler(16, fpu_handler);           // FPU Exception

    kprint("ISR initialization complete\n");
}