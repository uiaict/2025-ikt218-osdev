section .multiboot
align 8

; Multiboot2 header
header_start:
    dd 0xe85250d6                ; Magic number for multiboot2
    dd 0                         ; Architecture (protected mode i386)
    dd header_end - header_start ; Header length
    dd -(0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

    ; Required end tag
    dw 0    ; Type
    dw 0    ; Flags
    dd 8    ; Size
header_end:

section .text
bits 32
global _start
extern kernel_main

_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Call kernel main
    call kernel_main
    
    ; Halt if we return from kernel
.halt:
    cli
    hlt
    jmp .halt

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top: