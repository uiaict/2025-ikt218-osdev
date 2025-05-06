global gdt_flush

section .text
gdt_flush:
    mov eax, [esp + 4]   ; Get pointer to gdt_ptr
    lgdt [eax]

    mov ax, 0x10         ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs    ; Far jump to reload CS

flush_cs:
    ret
