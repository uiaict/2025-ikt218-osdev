BITS 32
section .text
global isr_stub_table

extern isr_handler

%macro ISR_STUB 1
global isr%1
isr%1:
    cli
    push dword 0             ; Dummy error code
    push dword %1            ; ISR number
    pusha
    mov eax, esp
    push eax
    call isr_handler
    add esp, 4
    popa
    add esp, 8
    sti
    iret
%endmacro

%assign i 0
%rep 32
    ISR_STUB i
%assign i i+1
%endrep

section .rodata
isr_stub_table:
%assign i 0
%rep 32
    extern isr%+i
    dd isr%+i
%assign i i+1
%endrep
