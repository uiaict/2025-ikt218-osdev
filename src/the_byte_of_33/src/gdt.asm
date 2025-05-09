; Loads a GDTR passed from C and switches to the new segments
[BITS 32]
global gdt_flush

gdt_flush:
    ; eax = &gdtr  (first C argument is at [esp+4] in cdecl)
    mov eax, [esp + 4]
    lgdt [eax]

    ; Reload data-segment registers
    mov ax, 0x10          ; selector = index 2 << 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to flush the prefetch queue and load CS = 0x08
    jmp 0x08:.flush_ok

.flush_ok:
    ret
