; filepath: /workspaces/2025-ikt218-osdev/src/70_AI_Alcatraz/src/gdt_flush.asm
[BITS 32]
global gdt_flush

; Function to load the GDT
gdt_flush:
    
    mov eax, [esp+4]
    lgdt [eax]
    
    ; Update the segment registers
    mov ax, 0x10      
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    
    jmp 0x08:.flush   

.flush:
    ret