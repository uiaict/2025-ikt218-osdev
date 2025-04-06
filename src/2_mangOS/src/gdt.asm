; EXTERN gdt_ptr  ; Imports gdt_ptr
; GLOBAL load_gdt ; Exports load_gdt

; section .text
; load_gdt:
;     mov eax, [esp+4] ; Get the address of the GDT
;     lgdt [gdt_ptr]

;     mov ax, 0x10 ; Set up data segments to point to data segment (0x10)

;     mov ds, ax ; Data segment
;     mov es, ax ; Extra segment
;     mov fs, ax ; FS segment
;     mov gs, ax ; GS segment
;     mov ss, ax ; Stack segment
    
;     jmp 0x08:.flush
; .flush:
;     ret

[GLOBAL gdt_flush]    ; Allows the C code to call gdt_flush().

gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret
