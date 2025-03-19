;Make the gdt_flush function globally accesible
global gdt_flush

gdt_flush:
    ; Retrieve the address of the gdt_ptr parameter
    MOV eax, [esp + 4]
    ;Load the GDT using the pointer in EAX
    lgdt [eax]

    ;Set segment registers (DS, ES, FS, GS, SS) to the kernel data segment
    MOV eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;Jump to the kernel code segment to flush
    JMP 0x08:.flush

.flush:
    ;Return to c from the function
    RET