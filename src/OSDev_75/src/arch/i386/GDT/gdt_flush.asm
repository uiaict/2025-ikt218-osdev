global gdt_flush
gdt_flush:
    MOV eax, [esp+4]
    LGDT [eax]
    MOV eax, 0x10  ; Kernel data selector
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax
    JMP 0x08:.flush  ; Kernel code selector
.flush:
    RET

global tss_flush
tss_flush:
    MOV ax, [esp+4]  ; TSS index
    SHL ax, 3        ; Multiply by 8 to get selector
    LTR ax
    RET