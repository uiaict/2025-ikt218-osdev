extern main

global _start

section .multiboot_header
header_start:
    dd 0xe85250d6 	                                                ; Magic number (multiboot 2)
    dd 0				                                            ; Architecture 0 (protected mode i386)
    dd header_end - header_start 	                                ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

;align 8
;framebuffer_tag_start:
;    dw 5                                              ; type
;    dw 1                                              ; flags
;    dd framebuffer_tag_end - framebuffer_tag_start    ; size
;    dd 800                                            ; width
;    dd 600                                            ; height
;    dd 32                                             ; depth
;framebuffer_tag_end:

align 8
    ; Required end tag:
    dw 0	; type
    dw 0	; flags
    dw 8	; size
header_end:

section .data 
 
struc mystruct
    .a resb 1
    .b resb 1
    .c resb 1
    .d resb 1
    .e resb 6
endstruc 


%define MYSTRUCT_SIZE 10

mystruct_instance resb MYSTRUCT_SIZE

section .text
bits 32

_start:
    cli

    mov esp, stack_top
    mov eax, mystruct_instance
    mov byte [ecx], 33
    mov byte [ecx + 1], 44
    mov byte [ecx + 2], 55
    mov byte [ecx + 3], 66
    mov byte [ecx + 4], 1 
    mov byte [ecx + 5], 2
    mov byte [ecx + 6], 3
    mov byte [ecx + 7], 4
    mov byte [ecx + 8], 5
    mov byte [ecx + 9], 6
      
 
	push ebx
	push eax
    push ecx
    call main ; Jump main function

section .bss
stack_bottom:
    resb 4096 * 16
stack_top:
