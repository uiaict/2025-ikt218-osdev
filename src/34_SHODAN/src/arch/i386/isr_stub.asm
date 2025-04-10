[BITS 32]

global isr0
global isr1
global isr2

extern isr_handler

section .text

isr0:
    cli                 ; Disable interrupts
    push 0              ; Push dummy error code (no real error code for ISR 0)
    push 0              ; Interrupt number
    call isr_handler
    add esp, 8          ; Clean up arguments from stack
    sti                 ; Re-enable interrupts
    iret                ; Return from interrupt

isr1:
    cli
    push 0
    push 1
    call isr_handler
    add esp, 8
    sti
    iret

isr2:
    cli
    push 0
    push 2
    call isr_handler
    add esp, 8
    sti
    iret
