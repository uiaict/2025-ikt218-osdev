global gdt_flush

section .text
gdt_flush:
    ; Get the pointer to the GDT descriptor
    mov eax, [esp + 4]
    
    ; Load the GDT
    lgdt [eax]
    
    ; Reload segment registers
    mov ax, 0x10    ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to reload CS (Code Segment)
    jmp 0x08:.reload_cs
    
.reload_cs:
    ret