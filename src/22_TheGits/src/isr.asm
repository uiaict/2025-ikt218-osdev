; === Egne ISRs for spesifikke interrupttyper ===

extern handle_syscall
extern handle_div_zero
global isr_syscall

section .text
bits 32


isr_syscall:
    cli
    pusha
    call handle_syscall
    popa
    sti
    iret

; === Generiske IRQ ISR-er ===
extern irq_handler

%macro ISR_IRQ 1
global isr_irq%1
isr_irq%1:
    cli
    pusha
    push %1
    call irq_handler
    add esp, 4
    popa
    sti
    iret
%endmacro

ISR_IRQ 0
ISR_IRQ 1
ISR_IRQ 2
ISR_IRQ 3
ISR_IRQ 4
ISR_IRQ 5
ISR_IRQ 6
ISR_IRQ 7
ISR_IRQ 8
ISR_IRQ 9
ISR_IRQ 10
ISR_IRQ 11
ISR_IRQ 12
ISR_IRQ 13
ISR_IRQ 14
ISR_IRQ 15

; === Default handler for udefinerte interrupts ===
extern default_int_handler

global default_isr
default_isr:
    cli
    pusha
    call default_int_handler
    popa
    sti
    iret


global isr_div_zero
isr_div_zero:
    cli
    pusha
    call handle_div_zero
    popa
    sti
    iret