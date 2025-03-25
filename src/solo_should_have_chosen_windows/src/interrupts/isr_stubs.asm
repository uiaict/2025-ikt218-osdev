[BITS 32]

global irq0_stub

extern irq_handler  ; C function we'll call

irq0_stub:
    pusha                     ; Push all general-purpose registers
    push dword 0              ; Push interrupt number (IRQ0 = 0) to stack
    call irq_handler          ; Call C handler with that number
    add esp, 4                ; Clean up pushed argument
    popa                      ; Restore registers
    iretd                     ; Return from interrupt
