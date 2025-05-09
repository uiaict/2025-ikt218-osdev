global gdt_flush     ; No underscore in your project
extern gdt_ptr       ; The variable name in your C code

gdt_flush:
    lgdt [gdt_ptr]    ; Load the GDT directly using the external symbol
    mov ax, 0x10      ; 0x10 is the offset to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment
.flush:
    ret               ; Return to C code