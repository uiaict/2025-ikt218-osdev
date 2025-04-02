extern isr_handler

global isr0_stub
global isr1_stub
global isr2_stub

section .text

isr0_stub:
    push 0
    call isr_handler
    add esp, 4
    iretd

isr1_stub:
    push 1
    call isr_handler
    add esp, 4
    iretd

isr2_stub:
    push 2
    call isr_handler
    add esp, 4
    iretd
