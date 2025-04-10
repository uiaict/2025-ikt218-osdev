#pragma once

void irq_init(void);
void irq_handler(int irq);
char keyboard_getchar(void);
void irq0_stub(void);
void pit_handler(void); // this is the C-level handler in pit.c


// Declare all IRQ stubs
void irq0_stub(void);
void irq1_stub(void);
void irq2_stub(void);
void irq3_stub(void);
void irq4_stub(void);
void irq5_stub(void);
void irq6_stub(void);
void irq7_stub(void);
void irq8_stub(void);
void irq9_stub(void);
void irq10_stub(void);
void irq11_stub(void);
void irq12_stub(void);
void irq13_stub(void);
void irq14_stub(void);
void irq15_stub(void);
