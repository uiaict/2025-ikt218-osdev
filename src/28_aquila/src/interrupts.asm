
[bits 32]

global isr0
global isr1
global isr2

extern isr_handler  

isr0:
    cli
    pusha
    call isr_handler
    popa
    sti
    iret

isr1:
    cli
    pusha
    call isr_handler
    popa
    sti
    iret

isr2:
    cli
    pusha
    call isr_handler
    popa
    sti
    iret
