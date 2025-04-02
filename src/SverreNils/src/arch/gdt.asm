global gdt_load

section .text
bits 32

; void gdt_load(struct gdt_ptr* gdt_descriptor)
gdt_load:
    mov eax, [esp + 4]      ; pointer to gdt_ptr
    lgdt [eax]              ; load GDT

    ; Update segment registers
    mov ax, 0x10            ; data segment selector (index 2, RPL 0)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs       ; far jump to code segment selector (index 1)

flush_cs:
    ret