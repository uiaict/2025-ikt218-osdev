    ; gdt_flush.asm
    global gdt_flush

gdt_flush:
    ; Load the GDT into the GDTR
    lgdt [esp]             ; Load the GDT pointer into the GDTR register
    
    ; Set the data segment registers (DS, ES, FS, GS)
    mov ax, 0x10           ; Code segment selector (index 1 in GDT)
    mov ds, ax             ; Set DS to the code segment
    mov es, ax             ; Set ES to the code segment
    mov fs, ax             ; Set FS to the code segment
    mov gs, ax             ; Set GS to the code segment
    mov ss, ax             ; Set SS to the code segment (for stack)
    
    ; Jump to the code segment (flush the CPU state)
    jmp 0x08:flush_next    ; Jump to the code segment, which is at index 1

flush_next:
    ret                    ; Return, now the CPU is in protected mode
