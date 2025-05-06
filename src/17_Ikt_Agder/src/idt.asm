[BITS 32]             ; Ensure NASM assembles in 32-bit mode

global idt_load       ; Export idt_load so C can call it
extern idt_ptr        ; IDT pointer is defined in C code

section .text
idt_load:
    lidt [idt_ptr]    ; Load IDT using lidt instruction
    ret               ; Return to caller
