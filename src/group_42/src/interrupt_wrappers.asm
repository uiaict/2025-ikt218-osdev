extern default_interrupt_handler
extern spurious_interrupt_handler
extern keyboard_handler

[global default_interrupt_handler_wrapper]
default_interrupt_handler_wrapper:
    pusha
    cld
    call default_interrupt_handler
    popa
    iret

[global spurious_interrupt_handler_wrapper]
spurious_interrupt_handler_wrapper:
    pusha
    cld
    call spurious_interrupt_handler
    popa
    iret

[global keyboard_handler_wrapper]
keyboard_handler_wrapper:
    pusha
    cld
    call keyboard_handler
    popa
    iret