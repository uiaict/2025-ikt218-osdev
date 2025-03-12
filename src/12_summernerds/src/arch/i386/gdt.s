.global gdt_flush
gdt_flush:
    lgdt (%eax)
    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0
    ret
