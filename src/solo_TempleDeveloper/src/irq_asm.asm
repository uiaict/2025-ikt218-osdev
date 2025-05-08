    ; === irq_asm.asm ===
    BITS 32
    EXTERN irq_handler
    EXTERN keyboard_handler

    SECTION .text

    %macro IRQ_STUB 1
    GLOBAL irq%1
    irq%1:
        pusha
        xor eax, eax        ; null ut resten
        mov al, %1          ; legg IRQ-nummer i AL
        cmp al, 1           ; er det IRQ1 (tastatur)?
        je .do_keyboard
        push eax            ; argument til irq_handler
        call irq_handler
        add esp, 4
        jmp .end_irq
    .do_keyboard:
        call keyboard_handler
    .end_irq:
        popa
        iret
    %endmacro

    ; Generate stubs irq0 … irq15
    IRQ_STUB 0
    IRQ_STUB 1
    IRQ_STUB 2
    IRQ_STUB 3
    IRQ_STUB 4
    IRQ_STUB 5
    IRQ_STUB 6
    IRQ_STUB 7
    IRQ_STUB 8
    IRQ_STUB 9
    IRQ_STUB 10
    IRQ_STUB 11
    IRQ_STUB 12
    IRQ_STUB 13
    IRQ_STUB 14
    IRQ_STUB 15
