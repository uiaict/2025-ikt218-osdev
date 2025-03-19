; filepath: /workspaces/2025-ikt218-osdev/src/70_AI_Alcatraz/src/gdt_flush.asm
[BITS 32]
global gdt_flush

; Function to load the GDT
gdt_flush:
    ; Get the pointer to the GDT, passed as parameter
    mov eax, [esp+4]
    ; Load the GDT pointer
    lgdt [eax]
    
    ; Update the segment registers
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to update CS register
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment

.flush:
    ret