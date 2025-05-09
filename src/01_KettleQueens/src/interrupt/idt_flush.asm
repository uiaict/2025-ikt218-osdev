[BITS 32]

global idt_flush

idt_flush:
    mov eax, [esp + 4]     ; Load the address of the IDT pointer (argument)
    lidt [eax]             ; Load the IDT
    sti                    ; Enable interrupts
    ret
