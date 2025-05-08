; gdt_flush.asm
global gdt_flush

gdt_flush:
    mov eax, [esp + 4]  ; Get gdt_ptr from the stack
    lgdt [eax]          ; Load it into the GDTR register

    mov ax, 0x10        ; Offset 0x10 = GDT entry 2 (data segment)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to load CS (code segment)
    jmp 0x08:.flush     ; 0x08 = GDT entry 1 (code segment)
.flush:
    ret
