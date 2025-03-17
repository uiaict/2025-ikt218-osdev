; filepath: /workspaces/2025-ikt218-osdev/src/OSDev_50/src/arch/i386/gdt_flush.asm
[BITS 32]                ; 32-bit code
global gdt_flush        ; Make function visible to C code

gdt_flush:
    mov eax, [esp+4]    ; Get pointer to GDT passed as argument
    lgdt [eax]          ; Load new GDT pointer

    ; Load new segment registers
    mov ax, 0x10        ; 0x10 is offset to data segment
    mov ds, ax          ; Set data segment registers
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 is offset to code segment
.flush:
    ret                 ; Return to C code