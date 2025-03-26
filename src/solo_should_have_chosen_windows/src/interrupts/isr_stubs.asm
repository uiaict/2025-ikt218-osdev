[BITS 32]

extern irq_handler        ; Declare the C function so we can call it

; Macro to generate ISR stub
%macro ISR_STUB 1
global isr_stub_%1
isr_stub_%1:
    pusha                 ; Push all general-purpose registers
    push dword %1         ; Push the interrupt number
    call irq_handler      ; Call common C handler
    add esp, 4            ; Clean up argument
    popa                  ; Restore registers
    iretd                 ; Return from interrupt
%endmacro

; Generate 256 stubs using macro
%assign i 0
%rep 256
    ISR_STUB i
%assign i i+1
%endrep

; Export the list of stub addresses (useful for C)
global isr_stubs
section .data
isr_stubs:
%assign i 0
%rep 256
    dd isr_stub_%+i
%assign i i+1
%endrep
