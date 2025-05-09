BITS 32
section .text
global irq_stub_table

%macro IRQ_STUB 1
global irq%1
irq%1:
    cli
    push dword 0
    push dword (32 + %1)
    jmp irq_common_stub
%endmacro

%assign i 0
%rep 16
    IRQ_STUB i
%assign i i+1
%endrep

extern irq_handler
irq_common_stub:
    pusha
    mov eax, esp
    push eax
    call irq_handler
    add esp, 4
    popa
    add esp, 8
    sti
    iret

section .rodata
irq_stub_table:
%assign i 0
%rep 16
    extern irq%+i
    dd irq%+i
%assign i i+1
%endrep
