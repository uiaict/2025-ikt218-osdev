.global isr0
.extern isr_handler

isr0:
    cli
    pusha                   ; Push general-purpose registers
    pushl $0                ; Fake error code (some ISRs don’t have one)
    pushl $0                ; Interrupt number
    call isr_handler
    add $8, %esp            ; Clean up stack (error code + int number)
    popa
    sti
    iret
