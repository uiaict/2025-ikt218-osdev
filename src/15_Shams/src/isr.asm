[GLOBAL isr0]
[GLOBAL isr1]
[GLOBAL isr2]
[EXTERN isr_handler]

isr0:
    cli
    push byte 0   ; Feilkode
    push byte 0   ; Interrupt nummer
    jmp isr_common_stub

isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

isr_common_stub:
    pusha
    mov ax, ds
    push eax      ; Lagre gammel datasement
    mov ax, 0x10  ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_handler
    pop eax
    mov ds, ax
    popa
    add esp, 8    ; Rydd opp i stack (feilkode + interrupt nummer)
    iret
