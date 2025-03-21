#include <libc/stdint.h>

// Function prototype for the exception handler
void exception_handler(void);

// Define the ISR handler functions with proper linkage
// These will be called when an interrupt occurs

// For ISRs that don't push an error code
#define ISR_NO_ERR(num) \
    __attribute__((naked)) void isr_stub_##num(void) { \
        __asm__ volatile( \
            "call exception_handler\n" \
            "iret\n" \
        ); \
    }

// For ISRs that push an error code
#define ISR_ERR(num) \
    __attribute__((naked)) void isr_stub_##num(void) { \
        __asm__ volatile( \
            "call exception_handler\n" \
            "iret\n" \
        ); \
    }

// Define each ISR handler function
// Exceptions 0-7 don't push error codes
ISR_NO_ERR(0)  // Divide by Zero
ISR_NO_ERR(1)  // Debug
ISR_NO_ERR(2)  // Non-maskable Interrupt
ISR_NO_ERR(3)  // Breakpoint
ISR_NO_ERR(4)  // Overflow
ISR_NO_ERR(5)  // Bound Range Exceeded
ISR_NO_ERR(6)  // Invalid Opcode
ISR_NO_ERR(7)  // Device Not Available

// Exception 8 pushes an error code
ISR_ERR(8)     // Double Fault

// Exception 9 doesn't push an error code
ISR_NO_ERR(9)  // Coprocessor Segment Overrun

// Exceptions 10-14 push error codes
ISR_ERR(10)    // Invalid TSS
ISR_ERR(11)    // Segment Not Present
ISR_ERR(12)    // Stack-Segment Fault
ISR_ERR(13)    // General Protection Fault
ISR_ERR(14)    // Page Fault

// Exceptions 15-31 don't push error codes
ISR_NO_ERR(15) // Reserved
ISR_NO_ERR(16) // x87 Floating-Point Exception
ISR_NO_ERR(17) // Alignment Check
ISR_NO_ERR(18) // Machine Check
ISR_NO_ERR(19) // SIMD Floating-Point Exception
ISR_NO_ERR(20) // Virtualization Exception
ISR_NO_ERR(21) // Control Protection Exception
ISR_NO_ERR(22) // Reserved
ISR_NO_ERR(23) // Reserved
ISR_NO_ERR(24) // Reserved
ISR_NO_ERR(25) // Reserved
ISR_NO_ERR(26) // Reserved
ISR_NO_ERR(27) // Reserved
ISR_NO_ERR(28) // Reserved
ISR_NO_ERR(29) // Reserved
ISR_NO_ERR(30) // Reserved
ISR_NO_ERR(31) // Reserved

// Create a table of function pointers to our ISR handlers
typedef void (*isr_fn)(void);
const isr_fn isr_stub_table[32] = {
    isr_stub_0,  isr_stub_1,  isr_stub_2,  isr_stub_3,
    isr_stub_4,  isr_stub_5,  isr_stub_6,  isr_stub_7,
    isr_stub_8,  isr_stub_9,  isr_stub_10, isr_stub_11,
    isr_stub_12, isr_stub_13, isr_stub_14, isr_stub_15,
    isr_stub_16, isr_stub_17, isr_stub_18, isr_stub_19,
    isr_stub_20, isr_stub_21, isr_stub_22, isr_stub_23,
    isr_stub_24, isr_stub_25, isr_stub_26, isr_stub_27,
    isr_stub_28, isr_stub_29, isr_stub_30, isr_stub_31
};