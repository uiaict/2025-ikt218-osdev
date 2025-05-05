[BITS 32]
global gdt_flush

gdt_flush:
    ; henter pekeren til GDT-strukturen (fra [esp+4])
    mov eax, [esp + 4]
    lgdt [eax]              ; last inn GDT-register

    ; sett alle segmentregister til datasegment (0x10)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; langt hopp til kodesegment (0x08) for å oppdatere CS
    jmp 0x08:.after_reload

.after_reload:
    ret
