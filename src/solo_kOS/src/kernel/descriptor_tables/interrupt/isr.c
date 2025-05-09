#include "kernel/interrupts.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

// ──────────────────────────────────────────────────────────────
// GLOBAL HANDLER TABLES
// ──────────────────────────────────────────────────────────────
// These tables store registered interrupt and IRQ handlers.
// They are used to look up the appropriate function when an interrupt occurs.

struct int_handler_t int_handlers[IDT_ENTRIES];  // For exceptions and software interrupts
struct int_handler_t irq_handlers[IRQ_COUNT];    // For hardware IRQs

// ──────────────────────────────────────────────────────────────
// REGISTER INTERRUPT HANDLER
// Used to install a handler function for a specific interrupt vector.
// This allows custom behavior (e.g., page fault handler, syscall hook, etc.)
// ──────────────────────────────────────────────────────────────

void register_interrupt_handler(uint8_t n, isr_t handler, void* context) {
    if (n < IDT_ENTRIES) {
        int_handlers[n].handler = handler;
        int_handlers[n].data = context;
        int_handlers[n].num = n;
    }
}

// ──────────────────────────────────────────────────────────────
// ISR HANDLER DISPATCHER
// This function is called from assembly when a CPU exception occurs.
// It checks whether a handler has been registered, and if so, calls it.
// If no handler is found, it halts the system (infinite loop).
// ──────────────────────────────────────────────────────────────

void isr_handler(registers_t regs)
{
    // Correct potential sign-extension on interrupt number.
    // Especially important for 0x80 (int 128), used in system calls.
    uint8_t int_no = regs.int_no & 0xFF;

    struct int_handler_t intrpt = int_handlers[int_no];
    if (intrpt.handler != 0) {
        intrpt.handler(&regs, intrpt.data);
    } else {
        // You could enable debug output here to diagnose unhandled ISRs.
        // For now, enter an infinite loop to halt the system.
        for (;;);
    }
}
