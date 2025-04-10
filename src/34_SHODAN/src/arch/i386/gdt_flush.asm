global gdt_flush

gdt_flush:
    mov eax, [esp + 4]    ; Load pointer to the GDT (passed from C)
    lgdt [eax]            ; Load GDT with 'lgdt'

    ; Reload all data segment registers with new selector (0x10 = GDT index 2)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to reload CS (code segment)
    jmp 0x08:.flush       ; 0x08 = GDT index 1 (code segment)

.flush:
    ret                   ; Return back to C after jump