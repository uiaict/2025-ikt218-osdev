[BITS 32]               ; Ensure NASM knows we're using 32-bit mode

global gdt_flush        ; Make gdt_flush available to C code

gdt_flush:
    mov eax, [esp+4]    ; Load the pointer to the GDT structure
    lgdt [eax]          ; Load the Global Descriptor Table


    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush     ; Far jump to reload CS (0x08 = code segment selector)
.flush:
    ret                 ; Return to C code
