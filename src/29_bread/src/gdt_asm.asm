global gdt_flush
gdt_flush:
    mov eax, [esp+4]  ; Get the GDT pointer argument
    lgdt [eax]        ; Load the GDT

    mov ax, 0x10      ; Kernel Data Segment (selector 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; Far jump to reload CS (0x08 = Kernel Code Segment)
.flush:
    ret