BITS 32
GLOBAL gdt_flush

gdt_flush:
    mov     eax, [esp+4]    ; &gdt_ptr (gdt_ptr_t*)
    lgdt    [eax]

    mov     ax, 0x10        ; kernel data‑segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    
    jmp     0x08:flush_done ; langt hopp til code‑segment 0x08
flush_done:
    ret


