#ifndef IDT_H
#define IDT_H

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();

#endif
