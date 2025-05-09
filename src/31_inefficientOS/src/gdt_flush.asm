global gdt_flush    ; Make the function accessible from C code

section .text
gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter
    lgdt [eax]        ; Load the new GDT pointer
    
    ; Reload CS register containing code selector:
    jmp 0x08:.reload_segments ; Far jump to the new code segment
    
.reload_segments:
    ; Reload data segment registers:
    mov ax, 0x10      ; 0x10 is the offset to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret