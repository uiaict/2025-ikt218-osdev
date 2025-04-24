global GDT_flush

GDT_flush:
    lgdt [esp+4]     
          
    mov ax, 0x10        
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush_2     

flush_2:
    ret
