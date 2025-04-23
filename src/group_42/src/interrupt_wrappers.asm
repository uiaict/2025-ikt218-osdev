extern default_interrupt_handler
extern spurious_interrupt_handler
extern keyboard_handler

[global default_interrupt_handler_wrapper]
default_interrupt_handler_wrapper:
    cli
    pusha
    cld
    call default_interrupt_handler
    popa
    sti
    iret

[global spurious_interrupt_handler_wrapper]
spurious_interrupt_handler_wrapper:
    cli
    pusha
    cld
    call spurious_interrupt_handler
    popa
    sti
    iret

[global keyboard_handler_wrapper]
keyboard_handler_wrapper:
    cli
    pusha
    cld
    call keyboard_handler
    popa
    sti
    iret