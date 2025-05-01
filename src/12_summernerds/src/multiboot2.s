    .extern main
    .globl _start

    .section .multiboot_header
    .globl header_start
header_start:
    .long 0xe85250d6                              # Magic number (multiboot 2)
    .long 0                                       # Architecture 0 (protected mode i386)
    .long header_end - header_start              # Header length
    .long 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) # Checksum

    # Optional framebuffer tag (commented out)
    # .align 8
    # framebuffer_tag_start:
    #     .short 5                                # type
    #     .short 1                                # flags
    #     .long framebuffer_tag_end - framebuffer_tag_start # size
    #     .long 800                               # width
    #     .long 600                               # height
    #     .long 32                                # depth
    # framebuffer_tag_end:

    .align 8
    .short 0                                      # type
    .short 0                                      # flags
    .long 8                                       # size
header_end:

    .section .text
    .code32

_start:
    cli

    movl $stack_top, %esp

    push %ebx
    push %eax

    call main

    .section .bss
    .align 4
stack_bottom:
    .space 4096 * 16
stack_top:
