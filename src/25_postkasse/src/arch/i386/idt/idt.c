#include "libc/stdint.h"
#include "libc/string.h"
#include "idt.h"

extern void isr_stub_0(void);
extern void isr_stub_1(void);
extern void isr_stub_2(void);

extern void idt_flush(uint32_t);

__attribute__((aligned(0x10))) 
idt_entry_t idt_entries[256];

void init_idt(){
    idtr.limit = sizeof(idt_entry_t) * 256 -1;
    idtr.base  = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; i++) {
        idt_entries[i].isr_low = 0;
        idt_entries[i].isr_high = 0;
        idt_entries[i].kernel_cs = 0;
        idt_entries[i].attributes = 0;
        idt_entries[i].reserved = 0;
    }

    idt_set_gate(0, (uint32_t)isr_stub_0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr_stub_1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr_stub_2, 0x08, 0x8E); 

    idt_flush((uint32_t)&idtr);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags){
    idt_entries[num].isr_low = base & 0xFFFF;
    idt_entries[num].isr_high = (base >> 16) & 0xFFFF;
    idt_entries[num].kernel_cs = sel;
    idt_entries[num].reserved = 0;
    idt_entries[num].attributes = flags;
}


__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt");
}

/*
void isr_common_handler(int interrupt_number) {
    switch (interrupt_number) {
        case 0:
            print("Interrupt 0\n");
            break;
        case 1:
            print("Interrupt 1\n");
            break;
        case 2:
            print("Interrupt 2\n");
            break;
        default:
            printf("Unknown interrupt");
    }
}
*/