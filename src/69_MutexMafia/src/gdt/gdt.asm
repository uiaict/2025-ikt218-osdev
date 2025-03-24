global gdt_flush

gdt_flush:
    mov eax, [esp+4]  ; Hent GDT-ptr fra argumentet
    lgdt [eax]        ; Last GDT

    mov ax, 0x10      ; Kernel data-segment (GDT entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; Langt hopp til kernel code-segment (GDT entry 1)
.flush:
    ret
