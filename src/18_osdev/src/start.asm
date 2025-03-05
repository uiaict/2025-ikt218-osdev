section .text
global gdt_flush   ; Expose it to C
extern gp          ; Ensure gp is accessible from C

gdt_flush:
    lgdt [gp]      ; Load the GDT using our pointer
    mov ax, 0x10   ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush2 ; 0x08 is the offset to our code segment: Far jump!
.flush2:
    ret             ; Return to C code
