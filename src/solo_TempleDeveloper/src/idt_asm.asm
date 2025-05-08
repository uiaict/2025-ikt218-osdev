BITS 32
GLOBAL idt_load
EXTERN isr_handler

SECTION .text

; idt_load: Load the IDT using lidt
; Expects pointer to idt_ptr on the stack (first argument)
idt_load:
    mov eax, [esp + 4]   ; Get address of idt_ptr
    lidt [eax]           ; Load IDT register
    ret

; ISR stub for interrupt 0
GLOBAL isr0
isr0:
    pusha                ; Save all general-purpose registers
    push byte 0          ; Push interrupt number (0) as argument
    call isr_handler     ; Call common C handler
    add esp, 4           ; Clean up stack argument
    popa                 ; Restore registers
    iret                 ; Return from interrupt

; ISR stub for interrupt 1
GLOBAL isr1
isr1:
    pusha
    push byte 1
    call isr_handler
    add esp, 4
    popa
    iret

; ISR stub for interrupt 2
GLOBAL isr2
isr2:
    pusha
    push byte 2
    call isr_handler
    add esp, 4
    popa
    iret
