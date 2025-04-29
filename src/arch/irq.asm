%macro IRQ 1
global irq%1
irq%1:
    cli
    push 0              ; dummy error code
    mov eax, 32 + %1    ; ✔️ legg inn int-nummer
    push eax
    jmp irq_common_stub
%endmacro


section .text
bits 32

extern irq_handler

irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
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

; 16 IRQ-stubber
%assign i 0
%rep 16
    IRQ i
    %assign i i+1
%endrep

; IRQ stub table
section .data
global irq_stub_table
irq_stub_table:
%assign j 0
%rep 16
    dd irq %+ j      ; ✅ Korrekt NASM-syntaks
    %assign j j+1
%endrep

