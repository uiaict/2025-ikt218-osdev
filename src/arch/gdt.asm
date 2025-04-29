global gdt_load

section .text
bits 32


gdt_load:
    mov eax, [esp + 4]      
    lgdt [eax]              

 
    mov ax, 0x10           
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs      

flush_cs:
    ret