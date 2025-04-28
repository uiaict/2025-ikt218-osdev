[bits 32]
[section .text]

extern irq_handler

; IRQ makro
%macro IRQ_HANDLER 2
global irq%1
irq%1:
    cli
    push byte 0         ; Dummy error code
    push byte %2        ; IRQ n + 32
    jmp common_irq_stub
%endmacro

; 32–33: IRQ0 (timer) ve IRQ1 (keyboard)

IRQ_HANDLER 0, 32
IRQ_HANDLER 1, 33



; felles IRQ kode
common_irq_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    sti
    iret
