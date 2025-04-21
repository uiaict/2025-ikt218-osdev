%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push 0              ; dummy error code
    push %1             ; interrupt number
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

    push esp            ; <--- push struct registers*
    call isr_handler
    add esp, 4          ; <--- rydde opp etter call

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret


; Generate all 256 stubs
%assign i 0
%rep 256
    ISR_NOERR i
    %assign i i+1
%endrep

; Build pointer table (isr_stub_table)
section .data
global isr_stub_table
isr_stub_table:
%assign j 0
%rep 256
    dd isr %+ j
    %assign j j+1
%endrep



