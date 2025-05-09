%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push 0              
    push %1             
    jmp isr_common_stub
%endmacro

section .text
bits 32

extern isr_handler

isr_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            
    call isr_handler
    add esp, 4          

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret



%assign i 0
%rep 256
    ISR_NOERR i
    %assign i i+1
%endrep


section .data
global isr_stub_table
isr_stub_table:
%assign j 0
%rep 256
    dd isr %+ j
    %assign j j+1
%endrep



