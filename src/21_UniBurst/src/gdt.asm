; gdt.asm
global gdt_flush

gdt_flush:
    mov eax, [esp+4]    ; Get the pointer to the GDT
    lgdt [eax]          ; Load the GDT
    
    ; Update data segment registers
    mov ax, 0x10        ; Offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to update CS register
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment
.flush:
    ret