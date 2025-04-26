[bits 32]
[section .text]

global gdt_flush  ; Export the gdt_flush symbol

gdt_flush:
    mov eax, [esp+4]  ; Load the pointer to GDT into eax
    lgdt [eax]        ; Load the GDT using lgdt instruction

    mov ax, 0x10      ; Data segment selector offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; Far jump to reload CS (code segment)
.flush:
    ret