; Multiboot2 header for NASM
section .multiboot_header
align 8
header_start:
    dd 0xE85250D6                                                   ; Multiboot2 magic number
    dd 0                                                            ; Architecture 0 (protected mode i386)
    dd header_end - header_start                                    ; Header length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start)) ; Checksum

    ; Required end tag
    align 8
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .bss
align 16
stack_bottom:
    resb 16384    ; 16 KiB of stack space
stack_top:

section .text
global _start
_start:
    ; Set up the stack
    mov esp, stack_top
    
    ; Pass multiboot info to main
    push ebx    ; multiboot info structure 
    push eax    ; multiboot magic number
    
    ; Call the C kernel main function
    extern main
    call main
    
    ; If main returns, enter an infinite loop
    cli
.hang:
    hlt
    jmp .hang