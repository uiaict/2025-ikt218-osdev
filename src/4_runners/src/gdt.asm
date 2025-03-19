global gdt_flush

section .text
gdt_flush:
    lgdt [eax]          ; Load the GDT pointer
    mov ax, 0x10        ; Load the data segment selector (index 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush_done ; Far jump to reload code segment (index 1)

.flush_done:
    ret