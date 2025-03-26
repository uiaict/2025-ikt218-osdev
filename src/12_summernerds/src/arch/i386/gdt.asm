global gdt_flush

gdt_flush:
    lgdt [eax]        ; Last GDT-deskriptoren fra adressen i EAX
    mov eax, cr0      ; Les CR0-registeret
    or eax, 1         ; Sett bit 0 (Protection Enable)
    mov cr0, eax      ; Skriv tilbake til CR0
    ret               ; Returner til kallende funksjon