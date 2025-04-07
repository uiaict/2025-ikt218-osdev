#include "libc/stdint.h"

#define M_PIC 0x20  // IO base address for master PIC
#define M_PIC_COMMAND M_PIC
#define M_PIC_DATA (M_PIC+1)

#define S_PIC 0xA0  // IO base address for slave PIC
#define S_PIC_COMMAND S_PIC
#define S_PIC_DATA (S_PIC+1)

#define PIC_EOI 0x20

struct registers{
    uint32_t ds;                                        // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by pusha.
    uint32_t int_no, err_code;                          // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed by the processor automatically.
};

void isr_handler(struct registers);

void irq_handler(struct registers);