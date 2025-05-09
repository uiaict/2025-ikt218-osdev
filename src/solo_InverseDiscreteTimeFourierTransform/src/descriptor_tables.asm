[GLOBAL gdt_load]

gdt_load:
    mov eax, [esp+4]  ; Copy  4-byte value from stack memory location [esp+4]
    lgdt [eax]        ; Load Global Descriptor Table Register (GDTR) with the GDT structure whose base address is stored in EAX

    jmp 0x08:.reload_cs   ; 0x08 is the offset to our code segment: Far jump!

.reload_cs:
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
