section .multiboot_header
align 8
header_start:
    dd 0xe85250d6                      ; Multiboot2 magic
    dd 0                               ; Architecture (i386)
    dd header_end - header_start       ; Length of header
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

align 8
    dw 0                               ; End tag: type = 0
    dw 0                               ; Flags = 0
    dd 8                               ; Size = 8
header_end:

section .text
bits 32
global _start
extern main

_start:
    cli
    mov esp, stack_top

    push ebx       ; Multiboot info
    push eax       ; Multiboot magic

    call main

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 4096 * 4
stack_top:
