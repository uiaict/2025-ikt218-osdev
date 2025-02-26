extern gdt_ptr  ; Imports gdt_ptr
global load_gdt ; Exports load_gdt

section .text
load_gdt:
    lgdt [gdt_ptr]
    
    jmp 0x08:flush_cs

flush_cs:
    mov ax, 0x10 ; 0x10 offset to our code segment

    mov ds, ax ; Data segment
    mov es, ax ; Extra segment
    mov fs, ax ; FS segment
    mov gs, ax ; GS segment
    mov ss, ax ; Stack segment
    
    ret