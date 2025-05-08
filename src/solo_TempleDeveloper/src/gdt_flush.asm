global gdt_flush

gdt_flush:
    mov eax, [esp + 4]    ; Get pointer to gdt_ptr passed from C
    lgdt [eax]            ; now load in the gdt from eax

    ; jump to reload control registers
    jmp 0x08:flush2       ; 0x08 is the selector for our code segment (value 16)


;Here we are essentially saying:
;"From now on, use the *GDT entry at index 2* (data segment) for all data and stack accesses"
flush2:
    mov ax, 0x10          ; 0x10 (value 16) is the selector for our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
