global GDT_flush

GDT_flush:
    lgdt [esp+4]        ; Load the GDT pointer from the stack

    mov ax, 0x10        ; Load data segment selector (index 2 in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_2    ; Far jump to reload CS with code segment selector (index 1)

flush_2:
    ret                 ; Return from function
