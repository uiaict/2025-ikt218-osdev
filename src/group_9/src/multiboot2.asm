global _start
extern kernel_main

section .multiboot_header
align 8
multiboot2_header:
    dd 0xe85250d6                ; Magic number (multiboot2)
    dd 0                         ; Architecture (0 = i386)
    dd multiboot2_header_end - multiboot2_header ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (multiboot2_header_end - multiboot2_header)) ; Checksum

align 8
    dw 0                         ; End tag type
    dw 0                         ; End tag flags
    dd 8                         ; End tag size
multiboot2_header_end:

section .text
bits 32

_start:
    cli                         ; Disable interrupts

    mov esp, stack_top
    push ebx                    ; multiboot info pointer
    push eax                    ; multiboot magic

    call kernel_main             ; Call your C kernel main

hang:
    hlt
    jmp hang

section .bss
align 16
stack_bottom:
    resb 4096 * 16
stack_top:
