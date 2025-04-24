#include "arch/isr.h"
#include "arch/idt.h"
#include "printf.h"

extern void* isr_stub_table[ISR_COUNT];

void isr_install() {
    for (int i = 0; i < ISR_COUNT; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }
}

void isr_handler(struct registers* regs) {
    printf("Interrupt %d (0x%x) triggered\n", regs->int_no, regs->int_no);
}

