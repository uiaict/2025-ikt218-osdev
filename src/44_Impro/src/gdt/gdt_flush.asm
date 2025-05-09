section .text
global gdt_flush

gdt_flush:
    mov eax, [esp + 4]   ; Load the first function argument (gdt_ptr)
    lgdt [eax]           ; Load the GDT descriptor from memory

    ; Perform a far jump to apply the new GDT
    jmp 0x08:.flush  ; 0x08 = Code Segment Selector (2nd entry in GDT)

.flush:
    mov ax, 0x10     ; 0x10 = Data Segment Selector (3rd entry in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ret
