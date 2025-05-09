section .text
global gdt_flush

gdt_flush:
    ; Load the new GDT pointer
    mov eax, [esp + 4]
    lgdt [eax]

    ; Update all data segment registers to use selector 0x10 (2nd GDT entry)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to flush the instruction pipeline and load CS with selector 0x08
    jmp 0x08:.flush

.flush:
    ret
