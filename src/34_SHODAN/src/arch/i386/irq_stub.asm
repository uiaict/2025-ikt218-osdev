%macro IRQ_HANDLER 1
global irq%1
irq%1:
    cli
    push byte 0          ; Dummy error code
    push byte %1         ; IRQ number
    jmp common_irq_handler
%endmacro

section .text

IRQ_HANDLER 0
IRQ_HANDLER 1
IRQ_HANDLER 2
IRQ_HANDLER 3
IRQ_HANDLER 4
IRQ_HANDLER 5
IRQ_HANDLER 6
IRQ_HANDLER 7
IRQ_HANDLER 8
IRQ_HANDLER 9
IRQ_HANDLER 10
IRQ_HANDLER 11
IRQ_HANDLER 12
IRQ_HANDLER 13
IRQ_HANDLER 14
IRQ_HANDLER 15

common_irq_handler:
    pusha
    extern irq_handler
    call irq_handler
    add esp, 8
    popa
    sti
    iret
