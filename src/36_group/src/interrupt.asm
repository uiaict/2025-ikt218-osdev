[BITS 32]

extern isr_handler
extern irq_handler
extern keyboard_handler

global isr0
global isr1
global isr2
global irq0_handler
global irq1_handler
global irq2_handler
global irq3_handler
global irq4_handler
global irq5_handler
global irq6_handler
global irq7_handler
global irq8_handler
global irq9_handler
global irq10_handler
global irq11_handler
global irq12_handler
global irq13_handler
global irq14_handler
global irq15_handler

%macro ISR_NOERR 1
isr%1:
    pusha
    push %1
    call isr_handler
    add esp, 4
    popa
    iret
%endmacro

%macro IRQ 1
irq%1_handler:
    pusha
    push %1
    call irq_handler
    add esp, 4
    popa
    iret
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2

IRQ 0
irq1_handler:
    pusha
    push 1
    call irq_handler
    add esp, 4
    popa
    iret
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15