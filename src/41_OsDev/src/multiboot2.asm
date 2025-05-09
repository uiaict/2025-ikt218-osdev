; multiboot2.asm
; Multiboot2 header and entry point for the kernel

extern main
global _start

section .multiboot_header
header_start:
    dd 0xe85250d6                            ; Multiboot2 magic number
    dd 0                                     ; Architecture: 0 = i386 protected mode
    dd header_end - header_start             ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

; Optional framebuffer tag (currently commented out)
;align 8
;framebuffer_tag_start:
;    dw 5                                    ; Tag type: framebuffer
;    dw 1                                    ; Flags: required
;    dd framebuffer_tag_end - framebuffer_tag_start ; Size of tag
;    dd 800                                  ; Width
;    dd 600                                  ; Height
;    dd 32                                   ; Bits per pixel
;framebuffer_tag_end:

align 8
    dw 0                                     ; End tag: type = 0
    dw 0                                     ; Flags = 0
    dw 8                                     ; Size = 8 bytes
header_end:

section .text
bits 32

_start:
    cli                                      ; Disable interrupts

    mov esp, stack_top                       ; Set up stack pointer

    push ebx                                 ; Push boot info (multiboot structure)
    push eax                                 ; Push multiboot magic

    call main                                ; Call C kernel entry function

section .bss
stack_bottom:
    resb 4096 * 16                           ; Reserve 64 KB for stack
stack_top:
