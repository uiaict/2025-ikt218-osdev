GLOBAL irq0
GLOBAL irq1
GLOBAL irq2

extern irq_handler

irq0:
    pusha
    mov eax, 0
    call irq_handler
    popa
    iret

irq1:
    pusha
    mov eax, 1
    call irq_handler
    popa
    iret

irq2:
    pusha
    mov eax, 2
    call irq_handler
    popa
    iret
