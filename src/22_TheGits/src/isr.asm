extern handle_timer_interrupt
extern handle_keyboard_interrupt
extern handle_syscall

global isr_timer
global isr_keyboard
global isr_syscall

section .text
bits 32

isr_timer:
    cli
    pusha
    call handle_timer_interrupt
    popa
    sti
    iret

isr_keyboard:
    cli
    pusha
    call handle_keyboard_interrupt
    popa
    sti
    iret

isr_syscall:
    cli
    pusha
    call handle_syscall
    popa
    sti
    iret