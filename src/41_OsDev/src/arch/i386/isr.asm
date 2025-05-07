;isr.asm
[bits 32]

global isr0
global isr1
global isr2
global idt_load

extern idtp
extern isr_handler

idt_load:
    lidt [idtp]
    ret

isr0:
    cli
    pusha
    push dword 0      ; interrupt number 0
    call isr_handler
    add esp, 4
    popa
    iret

isr1:
    cli
    pusha
    push dword 1
    call isr_handler
    add esp, 4
    popa
    iret

isr2:
    cli
    pusha
    push dword 2
    call isr_handler
    add esp, 4
    popa
    iret
