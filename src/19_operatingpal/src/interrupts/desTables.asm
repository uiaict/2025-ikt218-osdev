[GLOBAL gdtFlush]    ; Expose GDT flush to C

gdtFlush:
    mov eax, [esp+4]  ; Load pointer to GDT
    lgdt [eax]        ; Load GDT register

    mov ax, 0x10      ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; Far jump to reload CS
.flush:
    ret

[GLOBAL idtFlush]    ; Expose IDT flush to C

idtFlush:
    mov eax, [esp+4]  ; Load pointer to IDT
    lidt [eax]        ; Load IDT register
    ret
