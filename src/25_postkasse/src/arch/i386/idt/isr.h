#include "libc/stdint.h"

// Define IRQs for interrupt vectors 32 to 47
#define IRQ0  32
#define IRQ1  33 // Keyboard interrupt
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// Structure to store CPU registers during an interrupt
typedef struct registers {
      uint32_t ds;                                               // Data segment selector
      uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha
      uint32_t int_no, err_code;                                 // Interrupt number and error code
      uint32_t eip, cs, eflags, esp, ss;                         // Pushed by the processor automatically
  } registers_t;

// Define a type for interrupt service routines
typedef void (*isr_t)(registers_t);

//Function to register an interrupt handler
//n is the interrupt number, and handler is the function to handle it
void register_interrupt_handler(uint8_t n, isr_t handler);