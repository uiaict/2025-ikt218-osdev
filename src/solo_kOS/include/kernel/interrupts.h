#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../libc/stdint.h" // Fixed-width integer types (e.g., uint32_t)

// ──────────────────────────────────────────────────────────────
// CONSTANTS
// ──────────────────────────────────────────────────────────────

#define IDT_ENTRIES 256  // Total entries in the Interrupt Descriptor Table
#define IRQ_BASE    32   // PIC remaps IRQs to interrupts starting at 32
#define IRQ_COUNT   16   // Number of hardware IRQ lines

// ──────────────────────────────────────────────────────────────
// CPU REGISTER STATE STRUCTURE
// Used to store the CPU state pushed by our ISR/IRQ stubs.
// ──────────────────────────────────────────────────────────────

typedef struct registers {
    uint32_t ds;      // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no;  // Interrupt number
    uint32_t err_code;// Error code (0 for IRQs or non-error ISRs)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor
} registers_t;

// ──────────────────────────────────────────────────────────────
// C HANDLER TYPE
// ──────────────────────────────────────────────────────────────

typedef void (*isr_t)(registers_t* regs, void* context);  // Interrupt handler callback

// ──────────────────────────────────────────────────────────────
// HANDLER REGISTRATION STRUCTURE
// ──────────────────────────────────────────────────────────────

typedef struct int_handler_t {
    isr_t handler; // Function pointer to the handler
    void* data;    // Optional context data (can be NULL)
    int   num;     // Interrupt/IRQ number (for bookkeeping)
} int_handler_t;

// ──────────────────────────────────────────────────────────────
// ISR/IRQ HANDLER ARRAYS
// ──────────────────────────────────────────────────────────────

extern struct int_handler_t int_handlers[IDT_ENTRIES];  // For ISRs (exceptions)
extern struct int_handler_t irq_handlers[IRQ_COUNT];    // For hardware IRQs

// ──────────────────────────────────────────────────────────────
// EXTERN ASM STUBS FOR ISR/IRQ ROUTINES
// These are the entry points defined in assembly (e.g., isr_stub.asm)
// ──────────────────────────────────────────────────────────────

extern void isr0(),  isr1(),  isr2(),  isr3(),  isr4(),  isr5(),  isr6(),  isr7();
extern void isr8(),  isr9(),  isr10(), isr11(), isr12(), isr13(), isr14(), isr15();
extern void isr16(), isr17(), isr18(), isr19(), isr20(), isr21(), isr22(), isr23();
extern void isr24(), isr25(), isr26(), isr27(), isr28(), isr29(), isr30(), isr31();

extern void irq0(),  irq1(),  irq2(),  irq3(),  irq4(),  irq5(),  irq6(),  irq7();
extern void irq8(),  irq9(),  irq10(), irq11(), irq12(), irq13(), irq14(), irq15();

// ──────────────────────────────────────────────────────────────
// IDT STRUCTURES
// ──────────────────────────────────────────────────────────────

struct idt_entry_t {
    uint16_t base_low;   // Lower 16 bits of handler address
    uint16_t selector;   // Kernel segment selector (e.g., 0x08)
    uint8_t  zero;       // Reserved, must be 0
    uint8_t  flags;      // Type and attributes
    uint16_t base_high;  // Upper 16 bits of handler address
} __attribute__((packed));

struct idt_ptr_t {
    uint16_t limit; // Size of IDT - 1
    uint32_t base;  // Base address of IDT
} __attribute__((packed));

// ──────────────────────────────────────────────────────────────
// CORE INTERRUPT FUNCTIONS
// ──────────────────────────────────────────────────────────────

// IDT management
void init_idt();  // Initializes and loads the IDT

// C-level handlers
void isr_handler(registers_t regs);  // Handles CPU exceptions
void irq_handler(registers_t regs);  // Handles hardware IRQs

// Registration
void register_interrupt_handler(uint8_t n, isr_t handler, void* context);
void register_irq_handler(int irq, isr_t handler, void* context);
void unregister_irq_handler(int irq);
void init_irq();  // Initialize IRQ handler array

#endif // INTERRUPTS_H
