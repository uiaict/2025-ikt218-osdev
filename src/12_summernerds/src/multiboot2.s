.globl _start
.extern main

.section .multiboot_header
    .align 4
header_start:
    .long 0xe85250d6              # Magic number (multiboot2)
    .long 0                       # Architecture (0 = i386 protected mode)
    .long header_end - header_start  # Header length
    .long 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) # Checksum

    .align 8
    .short 0    # type
    .short 0    # flags
    .long 8     # size
header_end:

.section .text
.code32
.globl _start

_start:
    cli

    movl $stack_top, %esp

    pushl %ebx
    pushl %eax

    call main

.section .bss
    .align 4
stack_bottom:
    .space 4096 * 16
stack_top:
